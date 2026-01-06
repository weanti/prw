#ifndef WIDGET_H
#define WIDGET_H

#include "source.h"
#include "xconnection.h"

typedef struct 
{
    window_data wd;
    Source source;
    char* tooltip;
} Widget;

typedef struct
{
    Widget base;
    uint8_t padding[32];// this is s trick. This way "derived" structs can be mapped (cast) onto a Widget structure
} DerivedWidget;

Widget create_widget(   char* program,
                        char* tooltip,
                        window_data wd );
void draw_widget( Widget* );
void destroy_widget( Widget* );

#endif
