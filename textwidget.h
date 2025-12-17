#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "widget.h"

typedef struct 
{
    Widget base;
} TextWidget;

TextWidget create_textwidget(   int w, int h,
                                char* program,
                                char* tooltip,
                                xcb_connection_t* conn,
                                xcb_drawable_t win,
                                xcb_gcontext_t bg_ctx,
                                xcb_gcontext_t fg_ctx );
void draw_textwidget( Widget* );
void destroy_textwidget( Widget* );

#endif
