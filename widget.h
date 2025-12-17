#ifndef WIDGET_H
#define WIDGET_H

#include "source.h"

#include <xcb/xcb.h>

typedef struct 
{
    int w, h;
    Source source;
    char* tooltip;// owned
	xcb_connection_t* conn;
	xcb_drawable_t win;
	xcb_gcontext_t bg_ctx;
  	xcb_gcontext_t fg_ctx;
    uint8_t padding[32];
} Widget;

Widget create_widget( int w, int h, char* program, char* tooltip, xcb_connection_t* conn, xcb_drawable_t win, xcb_gcontext_t bg_ctx, xcb_gcontext_t fg_ctx );
void draw_widget( Widget* );
void destroy_widget( Widget* );

#endif
