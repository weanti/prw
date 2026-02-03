#ifndef XCONNECTION_DATA
#define XCONNECTION_DATA

#include <xcb/xcb.h>
#include <xcb/xproto.h>

typedef struct
{
    xcb_connection_t* conn;
    xcb_screen_t* screen; 
} session_data;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} geometry;

typedef struct
{
    session_data session;
    xcb_drawable_t win;
    xcb_gcontext_t bg_ctx;
    xcb_gcontext_t fg_ctx;
    int fg, bg;
} window_data;

session_data connect_display();
window_data create_window( session_data session, int w, int h, int bg, int fg, char* wmclass );
geometry get_geometry( window_data );

#endif
