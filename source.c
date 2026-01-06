#include "source.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char output[128];

Source create_source( char* program )
{
    Source source;
    source.program = strdup( program );
    strcpy( source.output_filename, "/tmp/prwXXXXXX" );
    source.output_filefd = mkstemp( source.output_filename );
    if ( source.output_filefd == -1 )
    {
        fprintf(stderr, "failed to open %s\n", source.output_filename);
        exit(1);
    }
    return source;
}

char* exec_source( Source source )
{
    int command_length = strlen(source.program) + strlen(" > ") + strlen(source.output_filename);
    char* command = (char*)malloc( (command_length+1) * sizeof(char) );
    strcpy( command, source.program );
    strcat( command, " > " );
    strcat( command, source.output_filename );
    int ret = system( command );
    free( command );
    if ( ret == -1 )
    {
        fprintf(stderr, "%s\n", strerror( errno ) );
        exit(1);
    }
    if ( ret == 127 )
    {
        fprintf( stderr, "unknown error\n" );
        exit(1);
    }
    // reposition to read the value
    lseek(source.output_filefd, 0, SEEK_SET );
    int count = read( source.output_filefd, output, 127 );
    output[count] = '\0';
    return output;
}

void destroy_source( Source source )
{
    close( source.output_filefd );
    unlink( source.output_filename );
}

