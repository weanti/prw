#include "tooltip_widget.h"

#include <xcb/xcb.h>

#include <string.h>

TextWidget create_tooltipwidget(   char* text,
                                       window_data wd )
{
    TextWidget tw;
    Source s = create_static_source( text );
    Widget w = { .wd = wd, .source = s };
    tw.base = w;
    xcb_font_t font_id = xcb_generate_id( tw.base.wd.session.conn );
    create_cairo_surface( &tw );

    char* eol = strpbrk( tw.base.source.source, "\n\r");
    if ( eol )
    {
        *eol = '\0';
    }
    return tw;
}

