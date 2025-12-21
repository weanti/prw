#ifndef BARWIDGET_H
#define BARWIDGET_H

#include "widget.h"

#include <xcb/xcb.h>

typedef struct
{
    Widget base; 
    double maxvalue; 
} BarWidget;

BarWidget create_barwidget( int w, int h,
                            char* program,
                            char* tooltip,
                            double maxvalue,
                            xcb_screen_t* screen, 
                            xcb_connection_t* conn,
                            xcb_drawable_t win,
                            xcb_gcontext_t bg_ctx,
                            xcb_gcontext_t fg_ctx );
void draw_barwidget( Widget* );
void destroy_barwidget( Widget* );

#endif
