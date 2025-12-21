#include "trendwidget.h"

#include <xcb/xcb.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

TrendWidget create_trendwidget( int w, int h,
                                char* program,
                                char* tooltip,
                                double maxvalue,
                                xcb_screen_t* screen, 
                                xcb_connection_t* conn,
                                xcb_drawable_t win,
                                xcb_gcontext_t bg_ctx,
                                xcb_gcontext_t fg_ctx )
{
    Widget widget = create_widget( w, h, program, tooltip, screen, conn, win, bg_ctx, fg_ctx );
    double* values = (double*)malloc( w*sizeof(double) );
    TrendWidget tw = { .widget = widget, .maxvalue = maxvalue, .values = values };
    return tw;
}

void draw_trendwidget( Widget* widget )
{
    TrendWidget* tw = ((TrendWidget*)widget);
    for ( int i = 0; i < widget->w-1; i++ )
    {
        tw->values[i] = tw->values[i+1];
    }
    double value = atof( exec_source( widget->source ) );
    // scale this value to [0, h()] interval using mMax value
    tw->values[widget->w-1] = fmin( widget->h, value / tw->maxvalue * widget->h );
    draw_widget( widget );
    _xcb_data xcb = widget->xcb;
    for ( int i = 0; i < widget->w-1; i++ )
    {
        // draw line
        xcb_point_t points[2] = { {.x = i, .y = widget->h}, {.x = 0, .y = -tw->values[i] } };
        xcb_poly_line(  xcb.conn,
                        XCB_COORD_MODE_PREVIOUS,
                        xcb.win,
                        xcb.fg_ctx,
                        2,
                        points );
    }
}

void destroy_trendwidget( Widget* widget )
{
    free( ((TrendWidget*)widget)->values );
    destroy_widget( widget );
}
