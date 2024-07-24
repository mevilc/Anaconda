#include "source.h"

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <cstdio>
#include <fstream>


const std::string runCmd( const char* cmd )
{
    char buf[ 128 ];
    std::string res;
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        return "";

    while (fgets(buf, sizeof(buf), pipe) != nullptr)
    {
        res += buf;

    }

    pclose(pipe);

    return res;
}


const bool diffDirectories( const std::string& dirA, const std::string& dirB )
{

    // error checks

    if ( dirB.empty() )
    {
        std::cerr << "Dir A not provided!\n";
        return false;
    }

    if ( dirB.empty() )
    {
        std::cerr << "Dir B not provided!\n";
        return false;

    }

    if ( ! std::filesystem::exists( dirA ) )
    {
        std::cerr << "Dir A does not exist!\n";
        return false;
    }

    if ( ! std::filesystem::exists( dirB ) )
    {
        std::cerr << "Dir B does nto exist!\n";
        return false;
    }

    // set up 3 files to write to parent directory (assuming its the current directory)
    std::string parentDir = dirA.substr(0, dirA.rfind("/"));
    std::ofstream commonFile(parentDir + "/common.txt");
    std::ofstream a_only(parentDir + "/a_only.txt");
    std::ofstream b_only(parentDir + "/b_only.txt");


    // generate md5sum checksums for each file in dir A
    // key: checksum, value: [filePath, T]
    // each path in dir A is True by default. If it exists in dir B too, make it F.

    std::unordered_map< std::string, DirAInfo > dirAChecksums;

    for( const auto& fileA : std::filesystem::directory_iterator{ dirA } )
    {
        if ( fileA.is_regular_file() )
        {
            std::string cmd = "md5sum " + fileA.path().string();
            auto res = runCmd( cmd.c_str() );
            if (res.empty())
            {
                std::cerr << "invalid system output\n";
                return false;
            }

            std::string checksum = res.substr(0, res.find(" "));
            dirAChecksums.insert({ checksum, DirAInfo( fileA.path().string(), true ) });
        }

    }


    // generate md5sum checksum for each file in dir B and check if it exists in dir A
    for( const auto& fileB : std::filesystem::directory_iterator{ dirB } )
    {
        if (fileB.is_regular_file())
        {

            std::string cmd = "md5sum " + fileB.path().string();
            auto res = runCmd( cmd.c_str() );
            if (res.empty())
            {
                std::cerr << "invalid system output\n";
                return false;
            }

            std::string checksum = res.substr(0, res.find(" "));

            // check
            if (dirAChecksums.count( checksum ) > 0)
            {
                // exists in both directories
                std::string fileInDirA = dirAChecksums.at(checksum).filePathA;
                dirAChecksums.at(checksum).existsOnlyInDirA = false;

                commonFile << fileInDirA << " " << fileB.path().string() << "\n";

            }

            else
            {
                // exists only in directory B
                b_only << fileB.path().string() << "\n";

            }

        }

    }

    // all files in dir A with a False are the files only found in dir A
    for ( const auto& pair : dirAChecksums )
    {
        if (pair.second.existsOnlyInDirA == true)
        {
            a_only << pair.second.filePathA << "\n";

        }

    }


    // close file handles
    commonFile.close();
    a_only.close();
    b_only.close();


    std::cout << "Completed diffing directories!\n";

    return true;
}