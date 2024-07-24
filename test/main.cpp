#include "source.h"
#include <iostream>

// testbed

int main( int argc, char** argv )
{
    if (argc < 3)
    {
        std::cerr << "insufficient args provided!\n";
        return -1;
    }

    std::string dirA( argv[1] );
    std::string dirB( argv[2] );

    if ( ! diffDirectories( dirA, dirB ) )
    {
        std::cerr << "unable to diff the 2 directories provided\n";
        return -1;
    }

    return 0;


}