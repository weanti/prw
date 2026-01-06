#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "widget.h"
#include "xconnection.h"

#include <cairo/cairo.h>
#include <pango/pango.h>

typedef struct 
{
    Widget base;
    int x, y;
    cairo_surface_t* surface;
    cairo_t* cr;
    PangoLayout* layout;
} TextWidget;

TextWidget create_textwidget(   char* program,
                                char* tooltip,
                                window_data xd );
void draw_textwidget( Widget* );
void destroy_textwidget( Widget* );

#endif
