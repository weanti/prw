#ifndef WIDGET_H
#define WIDGET_H

#include "source.h"
#include "xconnection.h"

typedef struct 
{
    Window* window;// parent, container object
    Source source;// input, the output of source is to be displayed in the widget
    char* tooltip;// tooltip text, optional
} Widget;

typedef struct
{
    Widget base;
    uint8_t padding[32];// this is s trick. This way "derived" structs can be mapped (cast) onto a Widget structure
} DerivedWidget;

Widget create_widget(   char* program,
                        char* tooltip );
// assigned widget to the parent window
void assign_widget( Widget* w, Window* parent );
void draw_widget( Widget* );
void destroy_widget( Widget* );

#endif
