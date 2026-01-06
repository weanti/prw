#include "widget.h"

#include <stdlib.h>

Widget create_widget(   char* program,
                        char* tooltip,
                        window_data wd )
{
    Source s = create_source( program );
    Widget widget = { .source = s, .tooltip = tooltip, .wd = wd };
    return widget;
}

void draw_widget( Widget* widget )
{
    // draws background box
    xcb_clear_area(widget->wd.session.conn,
            0, 
            widget->wd.win, 
            0, 
            0, 
            widget->wd.session.screen->width_in_pixels, 
            widget->wd.session.screen->height_in_pixels );
}

void destroy_widget( Widget* widget )
{
    free( widget->tooltip );
    destroy_source( widget->source );
}
