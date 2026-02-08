#ifndef XCONNECTION_DATA
#define XCONNECTION_DATA

#include <xcb/xcb.h>
#include <xcb/xproto.h>

// X connection details
typedef struct
{
    xcb_connection_t* conn;
    xcb_screen_t* screen; 
} Session;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} Geometry;

typedef struct
{
    Session session;
    xcb_drawable_t win;
    xcb_gcontext_t bg_ctx;
    xcb_gcontext_t fg_ctx;
    int fg, bg;// TODO: used directly?
} Window;

Session connect_display();
Window create_window( Session session, int x, int y, int w, int h, int bg, int fg, char* wmclass );
Geometry get_geometry( Window );
int is_mapped( Window );
xcb_visualtype_t* get_root_vt( xcb_screen_t* screen );

#endif
