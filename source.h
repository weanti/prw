#ifndef SOURCE_H
#define SOURCE_H

typedef struct 
{
    char* source;
    // the communication is done via files. For dynamic sources the given program's output is stored in this file and read from it.
    char output_filename[32];
    int output_filefd;
} Source;

// string is copied
Source create_static_source( char* program );
// string is copied
Source create_dynamic_source( char* text );
char* get( Source );
void destroy_source( Source );

#endif
