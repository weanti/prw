#include "source.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char output[128];

Source create_dynamic_source( char* source )
{
    Source s;
    s.source = strdup( source );
    strcpy( s.output_filename, "/tmp/prwXXXXXX" );
    s.output_filefd = mkstemp( s.output_filename );
    if ( s.output_filefd == -1 )
    {
        fprintf(stderr, "failed to open %s\n", s.output_filename);
        exit(1);
    }
    return s;
}

Source create_static_source( char* source )
{
    Source s;
    s.source = strdup( source );
    s.output_filefd = -1;
    return s;
}

char* execute( Source source )
{
    // static source
    if ( source.output_filefd == -1 )
    {
        return source.source;
    }
    int command_length = strlen(source.source) + strlen(" > ") + strlen(source.output_filename);
    char* command = (char*)malloc( (command_length+1) * sizeof(char) );
    strcpy( command, source.source);
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

