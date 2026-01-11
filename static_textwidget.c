#include "static_textwidget.h"

#include <xcb/xcb.h>

#include <string.h>

TextWidget create_static_textwidget(   char* text,
                                       char* tooltip,
                                       window_data wd )
{
    TextWidget tw;
    Source s = create_static_source( text );
    Widget w = { .wd = wd, .source = s, .tooltip = tooltip };
    tw.base = w;
    xcb_font_t font_id = xcb_generate_id( tw.base.wd.session.conn );
    create_cairo_surface( &tw );

    char* eol = strpbrk( tw.base.source.source, "\n\r");
    if ( eol )
    {
        *eol = '\0';
    }
    int width, height;
    // TODO: use a font priority list
    measure_size( tw.base.source.source, "Sans 8",  &width, &height, tw.layout );
   
    tw.x = (wd.width - width)/2;
    tw.y = (wd.height - height)/2;
    // TODO: now resize window?
    return tw;
}

