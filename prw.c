#include "barwidget.h"
#include "textwidget.h"
#include "trendwidget.h"

#include <xcb/xcb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int DEFAULT_WIDTH = 32;
const int DEFAULT_HEIGHT = 32;
const int DEFAULT_BG = 0x777777;
const int DEFAULT_FG = 0x0000BB;
const int DEFAULT_REPEAT = 1;
const int DEFAULT_MAXVALUE = 1;

void usage(const char* appname)
{
    printf("%s -b|-x|-r -source <source> [-w <width>] [-h <height>] [-fg <color>] [-bg <color>] [-repeat <interval>] [-maxvalue <value>] [-tooltip <text>]\n\
                -b: widget is a bar\n\
                -x: widget type is text\n\
                -r: widget type is trend, a value over time displayed as a bar chart\n\
                -source: an inline scrip, a script file or an executable with full command line. In case of text widget the output is interpreted as text, in case of trend widget it shall return a single number\n\
                -w: width of the widget in pixels. Default is %i\n\
                -h: height of the widget in pixels. Default is %i\n\
                -fg: foreground color in hex value. Text color or bar color. Format is the following: 0xRRGGBB. Default is %x\n\
                -bg: background color in hex value. Format is the following: 0xRRGGBB. Default is %x\n\
                -repeat: repeat interval in seconds. Default is %i\n\
                -maxvalue: used for trend widget and bar widget. The source script or program returns a value and maxvalue determines how high bar shall be drawn relative to th widget height. Default is %i\n\
                -tooltip: tooltip for the widget.\n", appname, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_FG, DEFAULT_BG, DEFAULT_REPEAT, DEFAULT_MAXVALUE );

}

int main(int argc, char** argv)
{
    int w = DEFAULT_WIDTH;
    int h = DEFAULT_HEIGHT;
    int fg = DEFAULT_FG;
    int bg = DEFAULT_BG;
    int repeat = DEFAULT_REPEAT;
    int maxvalue = DEFAULT_MAXVALUE;
    char* source = NULL;
    char* type = NULL;
    char* tooltip = NULL;
    for( int i = 1; i < argc; i++ )
    {
        if ( strcmp( argv[i], "-w" ) == 0 && i+1 < argc )
        {
            w = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-h" ) == 0 && i+1 < argc )
        {
            h = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-fg" ) == 0 && i+1 < argc )
        {
            sscanf( argv[i+1], "%x", &fg );
        }
        else if ( strcmp( argv[i], "-bg" ) == 0 && i+1 < argc )
        {
            sscanf( argv[i+1], "%x", &bg );
        }
        else if ( strcmp( argv[i], "-repeat" ) == 0 && i+1 < argc )
        {
            repeat = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-maxvalue" ) == 0 && i+1 < argc )
        {
            maxvalue = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-source" ) == 0 && i+1 < argc )
        {
            source = argv[i+1];
        }
        else if ( strcmp( argv[i], "-tooltip" ) == 0 && i+1 < argc )
        {
            tooltip = argv[i+1];
        }
        else if ( strcmp( argv[i], "-b" ) == 0 || strcmp( argv[i], "-x" ) == 0 || strcmp( argv[i], "-r" ) == 0 ) 
        {
            type = argv[i];
        }
        else if ( strcmp( argv[i], "-h" ) == 0 )
        {
            usage(argv[0]);
            return 0;
        }
    } 
    if ( ! source )
    {
        puts("-source is mandatory");
        usage(argv[0]);
        exit(1);
    }
    if ( ! type )
    {
        puts("-type is mandatory");
        usage(argv[0]);
        exit(1);
    }
    // connect
    xcb_connection_t* conn = xcb_connect( NULL, NULL );
    xcb_screen_t* screen = xcb_setup_roots_iterator( xcb_get_setup( conn ) ).data;
    
    // create background color context
    xcb_gcontext_t bg_ctx = xcb_generate_id( conn );
    uint32_t mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t values[2] = { (uint32_t)bg, 0 }; // background color
    xcb_create_gc( conn, bg_ctx, screen->root, mask, values );

    // create foreground (drawing) color context
    xcb_gcontext_t fg_ctx = xcb_generate_id( conn );
    mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = (uint32_t)fg; // drawing color
    values[1] = 0;
    xcb_create_gc( conn, fg_ctx, screen->root, mask, values );
        
    // create window
    xcb_drawable_t window = xcb_generate_id(conn);
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = bg;
    values[1] = XCB_EVENT_MASK_EXPOSURE;


    xcb_create_window(  conn,
                        XCB_COPY_FROM_PARENT,
                        window,
                        screen->root,
                        0,
                        0,
                        w,
                        h,
                        1,
                        XCB_WINDOW_CLASS_INPUT_OUTPUT,
                        screen->root_visual,
                        mask,
                        values );
    char * name = "PRW";
    xcb_change_property(conn,
                        XCB_PROP_MODE_APPEND,
                        window,
                        XCB_ATOM_WM_CLASS,
                        XCB_ATOM_STRING,
                        8,
                        strlen(name),
                        name );
    xcb_map_window(conn, window);
    
    // show
    xcb_flush(conn);
    
    Widget widget;
    void (*draw)(Widget*);// the widget specifig draw function
    if ( strcmp( type, "-b" ) == 0 )
    {
        BarWidget* bw = (BarWidget*)&widget;
        draw = draw_barwidget;
        *bw = create_barwidget( w, h, source, tooltip, maxvalue, conn, window, bg_ctx, fg_ctx );
    }
    else if ( strcmp( type, "-x" ) == 0 )
    {
        TextWidget* tw = (TextWidget*)&widget;
        draw = draw_textwidget;
        *tw = create_textwidget( w, h, source, tooltip, conn, window, bg_ctx, fg_ctx );
    }
    else if ( strcmp( type, "-r" ) == 0 )
    {
        TrendWidget* tw = (TrendWidget*)&widget;
        draw = draw_trendwidget;
        *tw = create_trendwidget( w, h, source, tooltip, maxvalue, conn, window, bg_ctx, fg_ctx );
    }
    xcb_generic_event_t* event;
    time_t last_update = time(NULL);
    while( 1 ) 
    {
        if ( (event = xcb_poll_for_event(conn) ) )
        {
            switch( event->response_type & ~0x80 )
            {
                case XCB_EXPOSE:
                {
                    draw( &widget );
                    xcb_flush(conn);
                }
                break;
                default:
                    break;

            }
            free(event);
            event = NULL;
        }

        time_t t = time(NULL);
        if ( t - last_update > repeat )
        {
            last_update = t;
            draw( &widget );
            xcb_flush(conn);
        }
        xcb_flush(conn);
        nanosleep( &(struct timespec){.tv_nsec = 500000000 }, NULL );
    }
    destroy_widget( &widget );
    xcb_disconnect( conn );
    return 0;
}
