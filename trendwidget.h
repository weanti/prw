#ifndef TRENDWIDGET_H
#define TRENDWIDGET_H

#include "widget.h"

typedef struct 
{
    Widget base;
    double* values;
    double maxvalue;
} TrendWidget;

TrendWidget create_trendwidget( char* program,
                                char* tooltip,
                                double maxvalue,
                                xcb_data xd );
void draw_trendwidget( Widget* );
void destroy_trendwidget( Widget* );

#endif
