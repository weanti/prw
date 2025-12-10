#include "source.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdexcept>

Source::Source( const std::string& source )
    : mSource( source )
{
    mOutputFileFD = mkstemp(mOutputFileName);
}

Source::Source( Source&& other )
{
    if ( &other != this )
    {
        mSource = other.mSource;
        std::copy( other.mOutputFileName, other.mOutputFileName + sizeof( other.mOutputFileName ), mOutputFileName );
        other.mOutputFileName[0] = '\0';
        mOutputFileFD = other.mOutputFileFD;
        other.mOutputFileFD = -1;
    }
}

Source::~Source()
{
    if ( mOutputFileFD != -1 )
    {
        close( mOutputFileFD );
        unlink( mOutputFileName );
    }
}

std::string Source::execute()
{
    char output[128];
    output[127] = '\0';
    std::string command = mSource + " > " + std::string(mOutputFileName);
    int ret = system( command.c_str() );
    if ( ret == -1 )
        throw std::runtime_error( std::string( strerror( errno ) ) );
    if ( ret == 127 )
        throw std::runtime_error( "unknown error" );
    lseek(mOutputFileFD, 0, SEEK_SET );
    int count = read( mOutputFileFD, output, 127 );
    output[count] = '\0';
    return std::string(output);
}
