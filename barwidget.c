#include "barwidget.h"

#include <math.h>
#include <stdlib.h>

BarWidget create_barwidget( char* program,
                            char* tooltip,
                            double maxvalue,
                            window_data xd )
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
    value = fmin( widget->wd.height, value / barwidget->maxvalue * widget->wd.height ); 
    window_data wd = widget->wd;
    xcb_rectangle_t rect[] = { 0, wd.height-value, (uint16_t)wd.width, value };
    xcb_poly_fill_rectangle(    wd.session.conn,
                                wd.win,
                                wd.fg_ctx,
                                1,
                                rect );
}

void destroy_barwidget( Widget* widget )
{
    destroy_widget( widget );
}
