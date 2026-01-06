#include "xconnection.h"

#include <xcb/xcb.h>

#include <string.h>

session_data connect_display()
{
    session_data session;
    session.conn = xcb_connect( NULL, NULL );
    session.screen = xcb_setup_roots_iterator( xcb_get_setup( session.conn ) ).data;
    return session;
}

window_data create_window( session_data session, int w, int h, int bg, int fg )
{
    window_data wd;
    wd.session = session;
    // create background color context
    wd.bg_ctx = xcb_generate_id(wd.session.conn);
    uint32_t mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t values[2] = { (uint32_t)bg, 0 }; // background color
    xcb_create_gc( wd.session.conn, wd.bg_ctx, wd.session.screen->root, mask, values );

    // create foreground (drawing) color context
    wd.fg_ctx = xcb_generate_id( wd.session.conn );
    mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = (uint32_t)fg; // drawing color
    values[1] = 0;
    xcb_create_gc( wd.session.conn, wd.fg_ctx, wd.session.screen->root, mask, values );

    wd.fg = fg;
    wd.bg = bg;
        
    // create window
    wd.win = xcb_generate_id(wd.session.conn);
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = bg;
    values[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(  wd.session.conn,
                        XCB_COPY_FROM_PARENT,
                        wd.win,
                        wd.session.screen->root,
                        0,
                        0,
                        w,
                        h,
                        1,
                        XCB_WINDOW_CLASS_INPUT_OUTPUT,
                        wd.session.screen->root_visual,
                        mask,
                        values );
    wd.width = w;
    wd.height = h;
    char * name = "PRW";
    xcb_change_property(wd.session.conn,
                        XCB_PROP_MODE_APPEND,
                        wd.win,
                        XCB_ATOM_WM_CLASS,
                        XCB_ATOM_STRING,
                        8,
                        strlen(name),
                        name );
    xcb_map_window(wd.session.conn, wd.win);
    return wd;
} 
