#include "textwidget.h"

#include <xcb/xcb.h>

#include <string.h>
#include <stdio.h>

TextWidget create_textwidget(   int w, int h,
                                char* program,
                                char* tooltip,
                                xcb_connection_t* conn,
                                xcb_drawable_t win,
                                xcb_gcontext_t bg_ctx,
                                xcb_gcontext_t fg_ctx )
{
    TextWidget tw;
    tw.base = create_widget( w, h, program, tooltip, conn, win, bg_ctx, fg_ctx );
    xcb_font_t font_id = xcb_generate_id( tw.base.conn );
    // TODO: try a list here
    //const char * name = "-adobe-times-medium-i-normal-0-0-100-100-p-0-iso88590-1";
    const char* name = "fixed";
    xcb_void_cookie_t cookie = xcb_open_font( tw.base.conn, font_id, strlen( name ), name );
    xcb_change_gc( tw.base.conn, tw.base.fg_ctx, XCB_GC_FONT, &cookie );
    xcb_close_font( tw.base.conn, font_id );
    
    return tw;
}

void draw_textwidget( Widget* widget )
{
    TextWidget* textwidget = (TextWidget*)widget;
    char* text = exec_source( widget->source );
    draw_widget( widget );
    xcb_image_text_8(   widget->conn,
                        strlen(text),
                        widget->win,
                        widget->fg_ctx,
                        0,
                        widget->h,
                        text );
}

void destroy_textwidget( Widget* widget )
{
    destroy_widget( widget );
}
