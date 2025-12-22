#ifndef WIDGET_H
#define WIDGET_H

#include "source.h"

#include <xcb/xcb.h>

typedef struct
{
    xcb_connection_t* conn;
    xcb_screen_t* screen; 
    xcb_drawable_t win;
    xcb_gcontext_t bg_ctx;
    xcb_gcontext_t fg_ctx;
    int width, height;
} xcb_data;

typedef struct 
{
    xcb_data xd;
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
                        xcb_data xd );
void draw_widget( Widget* );
void destroy_widget( Widget* );

#endif
