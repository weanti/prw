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

void measure_size( char* text, char* font, int* width, int* height, PangoLayout* layout );
void create_cairo_surface( TextWidget* tw );

void draw_textwidget( Widget* );
void destroy_textwidget( Widget* );

#endif
