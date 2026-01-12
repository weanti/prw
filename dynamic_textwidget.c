#include "dynamic_textwidget.h"

#include <xcb/xcb.h>

#include <string.h>

TextWidget create_dynamic_textwidget(   char* program,
                                        char* tooltip,
                                        window_data wd )
{
    TextWidget tw;
    tw.base = create_widget( program, tooltip, wd );
    xcb_font_t font_id = xcb_generate_id( tw.base.wd.session.conn );
    create_cairo_surface( &tw );

    char* text = get( tw.base.source );
    char* eol = strpbrk( text, "\n\r");
    if ( eol )
    {
        *eol = '\0';
    }
    int width, height;
    // TODO: use a font priority list
    measure_size( text, "Sans 8",  &width, &height, tw.layout );
   
    tw.x = (wd.width - width)/2;
    tw.y = (wd.height - height)/2;
    return tw;
}

