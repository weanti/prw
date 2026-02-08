#include "xconnection.h"

#include <xcb/xcb.h>

#include <stdlib.h>
#include <string.h>

Session connect_display()
{
    Session session;
    session.conn = xcb_connect( NULL, NULL );
    session.screen = xcb_setup_roots_iterator( xcb_get_setup( session.conn ) ).data;
    return session;
}

Window create_window( Session session, int x, int y, int w, int h, int bg, int fg, char* wmclass )
{
    Window wd;
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
    values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW;

    xcb_create_window(  wd.session.conn,
                        XCB_COPY_FROM_PARENT,
                        wd.win,
                        wd.session.screen->root,
                        x,
                        y,
                        w,
                        h,
                        1,
                        XCB_WINDOW_CLASS_INPUT_OUTPUT,
                        wd.session.screen->root_visual,
                        mask,
                        values );
    if ( wmclass )
    {
        xcb_change_property(wd.session.conn,
                            XCB_PROP_MODE_APPEND,
                            wd.win,
                            XCB_ATOM_WM_CLASS,
                            XCB_ATOM_STRING,
                            8,
                            strlen(wmclass),
                            wmclass );
    }
    return wd;
} 

Geometry get_geometry( Window wd )
{
    xcb_get_geometry_cookie_t geom_cookie = xcb_get_geometry( wd.session.conn, wd.win );
    xcb_get_geometry_reply_t* geom_reply = xcb_get_geometry_reply( wd.session.conn, geom_cookie, NULL );
    xcb_translate_coordinates_cookie_t translate_cookie = xcb_translate_coordinates( wd.session.conn, wd.win, wd.session.screen->root, 0, 0 );
    xcb_translate_coordinates_reply_t* translate_reply = xcb_translate_coordinates_reply( wd.session.conn, translate_cookie, NULL );

    Geometry g = { .x = translate_reply->dst_x, .y = translate_reply->dst_y, .width = geom_reply->width, .height = geom_reply->height };
    free(geom_reply);
    free(translate_reply);

    return g;
}

int is_mapped( Window wd )
{
    xcb_get_window_attributes_cookie_t cookie = xcb_get_window_attributes( wd.session.conn, wd.win );
    xcb_get_window_attributes_reply_t* attributes = xcb_get_window_attributes_reply( wd.session.conn, cookie, NULL );
    return (attributes->map_state != XCB_MAP_STATE_UNMAPPED);
}

xcb_visualtype_t* get_root_vt( xcb_screen_t* screen )
{
    xcb_visualtype_t* vt = NULL;
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator( screen );
    for ( ; !vt && depth_iter.rem; xcb_depth_next(&depth_iter) )
    {
        xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator( depth_iter.data );
        for ( ; !vt && visual_iter.rem; xcb_visualtype_next( &visual_iter ) )
        {
            if ( screen->root_visual == visual_iter.data->visual_id )
            {
                vt = visual_iter.data;
            }
        }
    }
    return vt;
}
