#ifndef BARWIDGET_H
#define BARWIDGET_H

#include "widget.h"
#include "xconnection.h"

// This widget displays a bar.
typedef struct
{
    Widget base; 
    double maxvalue; 
} BarWidget;

BarWidget create_barwidget( char* program,
                            char* tooltip,
                            double maxvalue,
                            window_data wd );
void draw_barwidget( Widget* );
void destroy_barwidget( Widget* );

#endif
