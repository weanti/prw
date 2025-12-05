#ifndef SOURCE_H
#define SOURCE_H

#include <string>

class Source
{
    public:
        Source( const std::string& source );
        Source( Source&& );
        ~Source();
        Source( const Source& ) = delete;
        Source& operator=( const Source& ) = delete;
        Source& operator=( Source&& ) = delete;

        // throws excetion in case of error
        std::string execute();
    private:
        std::string mSource;
        char mOutputFileName[15] = "/tmp/prwXXXXXX";
        int mOutputFileFD = -1;
};

#endif
