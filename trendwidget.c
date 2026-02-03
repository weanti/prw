#include "trendwidget.h"

#include <xcb/xcb.h>

#include <math.h>
#include <stdlib.h>

TrendWidget create_trendwidget( char* program,
                                char* tooltip,
                                double maxvalue )
{
    Widget base = create_widget( program, tooltip ); 
    TrendWidget tw = { .base = base, .values = NULL, .maxvalue = maxvalue };
    return tw;
}

void assign_trendwidget( TrendWidget* tw, window_data* parent )
{
    assign_widget( &tw->base, parent );
    geometry geom = get_geometry( *parent );
    double* values = (double*)malloc( geom.width*sizeof(double) );
    tw->values = values;
}

void draw_trendwidget( Widget* widget )
{
    TrendWidget* tw = ((TrendWidget*)widget);
    geometry geom = get_geometry( *(widget->window) );
    for ( int i = 0; i < geom.width-1; i++ )
    {
        tw->values[i] = tw->values[i+1];
    }
    double value = atof( get( widget->source ) );
    // scale this value to [0, h()] interval using mMax value
    tw->values[geom.width-1] = fmin( geom.height, value / tw->maxvalue * geom.height );
    draw_widget( widget );
    for ( int i = 0; i < geom.width; i++ )
    {
        // draw line
        xcb_point_t points[2] = { {.x = i, .y = geom.height }, {.x = 0, .y = -tw->values[i] } };
        xcb_poly_line(  widget->window->session.conn,
                        XCB_COORD_MODE_PREVIOUS,
                        widget->window->win,
                        widget->window->fg_ctx,
                        2,
                        points );
    }
}

void destroy_trendwidget( Widget* widget )
{
    free( ((TrendWidget*)widget)->values );
    destroy_widget( widget );
}
