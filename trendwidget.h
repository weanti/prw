#ifndef TRENDWIDGET_H
#define TRENDWIDGET_H

#include "widget.h"

typedef struct 
{
    Widget widget;
    double* values;
    double maxvalue;
} TrendWidget;

TrendWidget create_trendwidget( int w, int h,
                                char* program,
                                char* tooltip,
                                double maxvalue,
                                xcb_connection_t* conn,
                                xcb_drawable_t win,
                                xcb_gcontext_t bg_ctx,
                                xcb_gcontext_t fg_ctx );
void draw_trendwidget( Widget* );
void destroy_trendwidget( Widget* );

#endif
