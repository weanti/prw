#ifndef TRENDWIDGET_H
#define TRENDWIDGET_H

#include "widget.h"
#include "xconnection.h"

typedef struct 
{
    Widget base;
    double* values;
    double maxvalue;
} TrendWidget;

TrendWidget create_trendwidget( char* program,
                                char* tooltip,
                                double maxvalue,
                                window_data xd );
void draw_trendwidget( Widget* );
void destroy_trendwidget( Widget* );

#endif
