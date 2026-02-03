#include "barwidget.h"

#include <math.h>
#include <stdlib.h>

BarWidget create_barwidget( char* program,
                            char* tooltip,
                            double maxvalue,
                            window_data xd )
{
    Widget base = create_widget( program, tooltip, xd );
    BarWidget bw = { .base = base, .maxvalue = maxvalue };
    return bw;
}

void draw_barwidget( Widget* widget )
{
    draw_widget( widget );
    BarWidget* barwidget = (BarWidget*)widget; 
    double value = atof( get( widget->source ) );
    geometry geom = get_geometry( widget->wd );
    value = fmin( geom.height, value / barwidget->maxvalue * geom.height ); 
    xcb_rectangle_t rect[] = { 0, geom.height-value, (uint16_t)geom.width, value };
    xcb_poly_fill_rectangle(    widget->wd.session.conn,
                                widget->wd.win,
                                widget->wd.fg_ctx,
                                1,
                                rect );
}

void destroy_barwidget( Widget* widget )
{
    destroy_widget( widget );
}
