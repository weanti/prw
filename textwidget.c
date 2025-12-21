#include "textwidget.h"

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>
#include <pango/pangocairo.h>

#include <string.h>
#include <stdio.h>

void measure_size( char* text, char* font, int* width, int* height, TextWidget* tw )
{
    pango_layout_set_text(tw->layout, text, -1);

    PangoFontDescription *font_desc =
        pango_font_description_from_string(font);
    pango_layout_set_font_description(tw->layout, font_desc);

    /* --- Measure text --- */
    pango_layout_get_pixel_size(tw->layout, width, height);
}

void create_cairo_surface( TextWidget* tw )
{
    xcb_visualtype_t* vt = NULL;
    _xcb_data xcb = tw->base.xcb;
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator( xcb.screen );
    for ( ; !vt && depth_iter.rem; xcb_depth_next(&depth_iter) )
    {
        xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator( depth_iter.data );
        for ( ; !vt && visual_iter.rem; xcb_visualtype_next( &visual_iter ) )
        {
            if ( xcb.screen->root_visual == visual_iter.data->visual_id )
            {
                vt = visual_iter.data;
            }
        }
    }
    /* --- Cairo surface --- */
    tw->surface =
        cairo_xcb_surface_create(
            xcb.conn,
            xcb.win,
            vt,
            xcb.screen->width_in_pixels, xcb.screen->height_in_pixels
        );
    tw->cr = cairo_create(tw->surface);

    /* --- Pango layout --- */
    tw->layout = pango_cairo_create_layout(tw->cr);
}

TextWidget create_textwidget(   int w, int h,
                                char* program,
                                char* tooltip,
                                xcb_screen_t* screen, 
                                xcb_connection_t* conn,
                                xcb_drawable_t win,
                                xcb_gcontext_t bg_ctx,
                                xcb_gcontext_t fg_ctx )
{
    TextWidget tw;
    tw.base = create_widget( w, h, program, tooltip, screen, conn, win, bg_ctx, fg_ctx );
    xcb_font_t font_id = xcb_generate_id( tw.base.xcb.conn );
    create_cairo_surface( &tw );

    char* text = exec_source( tw.base.source );
    char* eol = strpbrk( text, "\n\r");
    if ( eol )
    {
        *eol = '\0';
    }
    int width, height;
    // TODO: use a font priority list
    measure_size( text, "Sans 8",  &width, &height, &tw );
   
    tw.x = (w - width)/2;
    tw.y = (h - height)/2;
    return tw;
}

void draw_textwidget( Widget* widget )
{
    TextWidget* textwidget = (TextWidget*)widget;
    char* text = exec_source( widget->source );
    draw_widget( widget );
    /* --- Draw text --- */
    cairo_set_source_rgb(textwidget->cr, 0, 0, 0);
    cairo_move_to(textwidget->cr, textwidget->x, textwidget->y);
    pango_cairo_show_layout(textwidget->cr, textwidget->layout);

    cairo_surface_flush(textwidget->surface);
    xcb_flush(textwidget->base.xcb.conn);
}

void destroy_textwidget( Widget* widget )
{
    destroy_widget( widget );
}
