#include "barwidget.h"

#include <math.h>
#include <stdlib.h>

BarWidget create_barwidget( int w, int h,
                            char* program,
                            char* tooltip,
                            double maxvalue,
                            xcb_screen_t* screen, 
                            xcb_connection_t* conn,
                            xcb_drawable_t win,
                            xcb_gcontext_t bg_ctx,
                            xcb_gcontext_t fg_ctx )
{
    Widget base = create_widget( w, h, program, tooltip, screen, conn, win, bg_ctx, fg_ctx );
    BarWidget bw = { .base = base, maxvalue = maxvalue };
    return bw;
}

void draw_barwidget( Widget* widget )
{
    draw_widget( widget );
    BarWidget* barwidget = (BarWidget*)widget; 
    double value = atof( exec_source( widget->source ) );
    value = fmin( widget->h, value / barwidget->maxvalue * widget->h ); 
    _xcb_data xcb = widget->xcb;
    xcb_rectangle_t rect[] = { 0, widget->h-value, (uint16_t)widget->w, value };
    xcb_poly_fill_rectangle(    xcb.conn,
                                xcb.win,
                                xcb.fg_ctx,
                                1,
                                rect );
}

void destroy_barwidget( Widget* widget )
{
    destroy_widget( widget );
}
