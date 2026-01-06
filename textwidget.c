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

TextWidget create_textwidget(   char* program,
                                char* tooltip,
                                window_data wd )
{
    TextWidget tw;
    tw.base = create_widget( program, tooltip, wd );
    xcb_font_t font_id = xcb_generate_id( tw.base.wd.session.conn );
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
   
    tw.x = (wd.width - width)/2;
    tw.y = (wd.height - height)/2;
    return tw;
}

void draw_textwidget( Widget* widget )
{
    printf( "drwaing %llu\n", time(NULL) );
    TextWidget* textwidget = (TextWidget*)widget;
    char* text = exec_source( widget->source );
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
