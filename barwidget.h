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
                            double maxvalue );
// assign widget to the parent window and perform barwidget specific initializations 
void assign_barwidget( BarWidget* bw, Window* parent );
void draw_barwidget( Widget* );
void destroy_barwidget( Widget* );

#endif
