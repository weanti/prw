#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "widget.h"

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

TextWidget create_textwidget(   int w, int h,
                                char* program,
                                char* tooltip,
                                xcb_screen_t* screen, 
                                xcb_connection_t* conn,
                                xcb_drawable_t win,
                                xcb_gcontext_t bg_ctx,
                                xcb_gcontext_t fg_ctx );
void draw_textwidget( Widget* );
void destroy_textwidget( Widget* );

#endif
