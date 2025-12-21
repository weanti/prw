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
} _xcb_data;

typedef struct 
{
    int w, h;
    _xcb_data xcb;
    Source source;
    char* tooltip;// owned
    uint8_t padding[32];// this is s trick. This way "derived" structs can be mapped (cast) onto a Widget structure
} Widget;

Widget create_widget(   int w, int h,
                        char* program,
                        char* tooltip,
                        xcb_screen_t* screen,
                        xcb_connection_t* conn,
                        xcb_drawable_t win,
                        xcb_gcontext_t bg_ctx,
                        xcb_gcontext_t fg_ctx );
void draw_widget( Widget* );
void destroy_widget( Widget* );

#endif
