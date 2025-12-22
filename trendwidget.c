#include "trendwidget.h"

#include <xcb/xcb.h>

#include <math.h>
#include <stdlib.h>

TrendWidget create_trendwidget( char* program,
                                char* tooltip,
                                double maxvalue,
                                xcb_data xd )
{
    Widget base = create_widget( program, tooltip, xd ); 
    double* values = (double*)malloc( xd.width*sizeof(double) );
    TrendWidget tw = { .base = base, .maxvalue = maxvalue, .values = values };
    return tw;
}

void draw_trendwidget( Widget* widget )
{
    TrendWidget* tw = ((TrendWidget*)widget);
    for ( int i = 0; i < widget->xd.width-1; i++ )
    {
        tw->values[i] = tw->values[i+1];
    }
    double value = atof( exec_source( widget->source ) );
    // scale this value to [0, h()] interval using mMax value
    tw->values[widget->xd.width-1] = fmin( widget->xd.height, value / tw->maxvalue * widget->xd.height );
    draw_widget( widget );
    xcb_data xd = widget->xd;
    for ( int i = 0; i < widget->xd.width-1; i++ )
    {
        // draw line
        xcb_point_t points[2] = { {.x = i, .y = widget->xd.height }, {.x = 0, .y = -tw->values[i] } };
        xcb_poly_line(  xd.conn,
                        XCB_COORD_MODE_PREVIOUS,
                        xd.win,
                        xd.fg_ctx,
                        2,
                        points );
    }
}

void destroy_trendwidget( Widget* widget )
{
    free( ((TrendWidget*)widget)->values );
    destroy_widget( widget );
}
