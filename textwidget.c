#include "textwidget.h"

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>
#include <pango/pangocairo.h>

#include <string.h>
#include <stdio.h>

void measure_size( char* text, char* font, int* width, int* height, PangoLayout* layout )
{
    pango_layout_set_text(layout, text, -1);

    PangoFontDescription *font_desc =
        pango_font_description_from_string(font);
    pango_layout_set_font_description(layout, font_desc);

    /* --- Measure text --- */
    pango_layout_get_pixel_size(layout, width, height);
}

void create_cairo_surface( TextWidget* tw )
{
    xcb_visualtype_t* vt = NULL;
    window_data wd= tw->base.wd;
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator( wd.session.screen );
    for ( ; !vt && depth_iter.rem; xcb_depth_next(&depth_iter) )
    {
        xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator( depth_iter.data );
        for ( ; !vt && visual_iter.rem; xcb_visualtype_next( &visual_iter ) )
        {
            if ( wd.session.screen->root_visual == visual_iter.data->visual_id )
            {
                vt = visual_iter.data;
            }
        }
    }
    /* --- Cairo surface --- */
    tw->surface =
        cairo_xcb_surface_create(
            wd.session.conn,
            wd.win,
            vt,
            wd.width, wd.height
        );
    tw->cr = cairo_create(tw->surface);

    /* --- Pango layout --- */
    tw->layout = pango_cairo_create_layout(tw->cr);
}

void draw_textwidget( Widget* widget )
{
    TextWidget* textwidget = (TextWidget*)widget;
    char* text = execute( widget->source );
    draw_widget( widget );
    /* --- Draw text --- */
    int r = (widget->wd.fg >> 16) & 0xFF;
    int g = (widget->wd.fg >> 8) & 0xFF;
    int b = widget->wd.fg & 0xFF;
    cairo_set_source_rgb(textwidget->cr, r, g, b);
    cairo_move_to(textwidget->cr, textwidget->x, textwidget->y);
    pango_cairo_show_layout(textwidget->cr, textwidget->layout);
    pango_layout_set_text(textwidget->layout, text, -1);

    cairo_surface_flush(textwidget->surface);
}

void destroy_textwidget( Widget* widget )
{
    destroy_widget( widget );
}
