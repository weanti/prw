#ifndef SOURCE_H
#define SOURCE_H

typedef struct 
{
    char* program;
    char output_filename[32];
    int output_filefd;
} Source;

// string is copied
Source create_source( char* program );
char* exec_source( Source );
void destroy_source( Source );

#endif
