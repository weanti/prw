#include "widget.h"

#include <stdlib.h>

Widget create_widget( char* program )
{
    Source s = create_dynamic_source( program );
    Widget widget = { .window = NULL, .source = s };
    return widget;
}

void assign_widget( Widget* w, Window* parent )
{
    w->window = parent;
}

void draw_widget( Widget* widget )
{
    // draws background box
    xcb_clear_area(widget->window->session.conn,
            0, 
            widget->window->win, 
            0, 
            0, 
            widget->window->session.screen->width_in_pixels, 
            widget->window->session.screen->height_in_pixels );
}

void destroy_widget( Widget* widget )
{
    destroy_source( widget->source );
}
