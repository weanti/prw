#include "tooltip_widget.h"

#include <cairo/cairo-xcb.h>
#include <xcb/xcb.h>

#include <string.h>

void resize_widget( TextWidget* tw )
{
    int width, height;
    // TODO: use a font priority list
    measure_size( tw->base.source.source, "Sans 8",  &width, &height, tw->layout );
   
    // resize cairo surface and window
    uint32_t new_size[2] = { width+2, height+2 };
    cairo_xcb_surface_set_size( tw->surface, width+2, height+2 );
    xcb_configure_window( tw->base.wd.session.conn, tw->base.wd.win, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, new_size );
    // there is a 1 px adding on each side
    tw->x = 1;
    tw->y = 1;
}

TextWidget create_tooltip_widget(   char* text,
                                       window_data wd )
{
    TextWidget tw;
    Source s = create_static_source( text );
    Widget w = { .wd = wd, .source = s };
    tw.base = w;
    tw.x = 0;
    tw.y = 0;
    xcb_font_t font_id = xcb_generate_id( tw.base.wd.session.conn );
    create_cairo_surface( &tw );

    char* eol = strpbrk( tw.base.source.source, "\n\r");
    if ( eol )
    {
        *eol = '\0';
    }
   
    return tw;
}

