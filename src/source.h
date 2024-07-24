#ifndef _DIRECTORY_CHECK_
#define _DIRECTORY_CHECK_

#include <string>

struct DirAInfo
{
        std::string filePathA;
        bool existsOnlyInDirA;

        DirAInfo(std::string filePathA_, bool existsOnlyInDirA_) :
        filePathA(filePathA_), existsOnlyInDirA(existsOnlyInDirA_) { }
};

const bool diffDirectories( const std::string& dir1, const std::string& dir2 );

#endif