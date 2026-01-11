#include "trendwidget.h"

#include <xcb/xcb.h>

#include <math.h>
#include <stdlib.h>

TrendWidget create_trendwidget( char* program,
                                char* tooltip,
                                double maxvalue,
                                window_data wd )
{
    Widget base = create_widget( program, tooltip, wd ); 
    double* values = (double*)malloc( wd.width*sizeof(double) );
    TrendWidget tw = { .base = base, .values = values, .maxvalue = maxvalue };
    return tw;
}

void draw_trendwidget( Widget* widget )
{
    TrendWidget* tw = ((TrendWidget*)widget);
    for ( int i = 0; i < widget->wd.width-1; i++ )
    {
        tw->values[i] = tw->values[i+1];
    }
    double value = atof( execute( widget->source ) );
    // scale this value to [0, h()] interval using mMax value
    tw->values[widget->wd.width-1] = fmin( widget->wd.height, value / tw->maxvalue * widget->wd.height );
    draw_widget( widget );
    for ( int i = 0; i < widget->wd.width; i++ )
    {
        // draw line
        xcb_point_t points[2] = { {.x = i, .y = widget->wd.height }, {.x = 0, .y = -tw->values[i] } };
        xcb_poly_line(  widget->wd.session.conn,
                        XCB_COORD_MODE_PREVIOUS,
                        widget->wd.win,
                        widget->wd.fg_ctx,
                        2,
                        points );
    }
}

void destroy_trendwidget( Widget* widget )
{
    free( ((TrendWidget*)widget)->values );
    destroy_widget( widget );
}
