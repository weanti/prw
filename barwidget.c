#include "barwidget.h"

#include <math.h>
#include <stdlib.h>

BarWidget create_barwidget( char* program,
                            double maxvalue )
{
    Widget base = create_widget( program );
    BarWidget bw = { .base = base, .maxvalue = maxvalue };
    return bw;
}

void assign_barwidget( BarWidget* bw, Window* parent )
{
    assign_widget( &bw->base, parent );
}

void draw_barwidget( Widget* widget )
{
    draw_widget( widget );
    BarWidget* barwidget = (BarWidget*)widget; 
    double value = atof( get( widget->source ) );
    Geometry geom = get_geometry( *(widget->window) );
    value = fmin( geom.height, value / barwidget->maxvalue * geom.height ); 
    xcb_rectangle_t rect[] = { 0, geom.height-value, (uint16_t)geom.width, value };
    xcb_poly_fill_rectangle(    widget->window->session.conn,
                                widget->window->win,
                                widget->window->fg_ctx,
                                1,
                                rect );
}

void destroy_barwidget( Widget* widget )
{
    destroy_widget( widget );
}
