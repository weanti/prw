#ifndef SOURCE_H
#define SOURCE_H

typedef struct 
{
    char* source;
    char output_filename[32];
    int output_filefd;
} Source;

// string is copied
Source create_static_source( char* source );
// string is copied
Source create_dynamic_source( char* source );
char* execute( Source );
void destroy_source( Source );

#endif
