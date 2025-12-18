#include "widget.h"

#include <stdlib.h>

Widget create_widget(   int w,
                        int h,
                        char* program,
                        char* tooltip,
                        xcb_connection_t* conn,
                        xcb_drawable_t win,
                        xcb_gcontext_t bg_ctx,
                        xcb_gcontext_t fg_ctx )
{
    Source s = create_source( program );
    Widget widget = { .w = w, .h = h, .source = s, .conn = conn, .win = win, .bg_ctx = bg_ctx, .fg_ctx = fg_ctx };
    return widget;
}

void draw_widget( Widget* widget )
{
    // draws background box
    xcb_clear_area( widget->conn, 0, widget->win, 0, 0, widget->w, widget->h );
}

void destroy_widget( Widget* widget )
{
    free( widget->tooltip );
    destroy_source( widget->source );
}
