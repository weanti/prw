#ifndef BARWIDGET_H
#define BARWIDGET_H

#include "widget.h"

#include <xcb/xcb.h>

typedef struct
{
    Widget base; 
    double maxvalue; 
} BarWidget;

BarWidget create_barwidget( char* program,
                            char* tooltip,
                            double maxvalue,
                            xcb_data xd );
void draw_barwidget( Widget* );
void destroy_barwidget( Widget* );

#endif
