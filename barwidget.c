#include "barwidget.h"

#include <math.h>
#include <stdlib.h>

BarWidget create_barwidget( char* program,
                            char* tooltip,
                            double maxvalue,
                            xcb_data xd )
{
    Widget base = create_widget( program, tooltip, xd );
    BarWidget bw = { .base = base, maxvalue = maxvalue };
    return bw;
}

void draw_barwidget( Widget* widget )
{
    draw_widget( widget );
    BarWidget* barwidget = (BarWidget*)widget; 
    double value = atof( exec_source( widget->source ) );
    value = fmin( widget->xd.height, value / barwidget->maxvalue * widget->xd.height ); 
    xcb_data xd = widget->xd;
    xcb_rectangle_t rect[] = { 0, xd.height-value, (uint16_t)xd.width, value };
    xcb_poly_fill_rectangle(    xd.conn,
                                xd.win,
                                xd.fg_ctx,
                                1,
                                rect );
}

void destroy_barwidget( Widget* widget )
{
    destroy_widget( widget );
}
