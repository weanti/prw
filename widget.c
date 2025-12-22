#include "widget.h"

#include <stdlib.h>

Widget create_widget(   char* program,
                        char* tooltip,
                        xcb_data xd )
{
    Source s = create_source( program );
    Widget widget = { .source = s, .tooltip = tooltip, .xd = xd };
    return widget;
}

void draw_widget( Widget* widget )
{
    // draws background box
    xcb_clear_area(widget->xd.conn,
            0, 
            widget->xd.win, 
            0, 
            0, 
            widget->xd.screen->width_in_pixels, 
            widget->xd.screen->height_in_pixels );
}

void destroy_widget( Widget* widget )
{
    free( widget->tooltip );
    destroy_source( widget->source );
}
