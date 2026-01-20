#include "barwidget.h"
#include "textwidget.h"
#include "tooltip_widget.h"
#include "trendwidget.h"
#include "xconnection.h"

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

// fills passed parameters based on the command line args
// returns 1 if parse is OK, 0 otherwise
int parse_args(    int argc, char** argv,
                   int* w, int* h,
                   int* fg, int* bg,
                   int* repeat,
                   int* maxvalue,
                   char* source,
                   char* type,
                   char* tooltip )
{
    for( int i = 1; i < argc; i++ )
    {
        if ( strcmp( argv[i], "-w" ) == 0 && i+1 < argc )
        {
            *w = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-h" ) == 0 && i+1 < argc )
        {
            *h = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-fg" ) == 0 && i+1 < argc )
        {
            sscanf( argv[i+1], "%x", fg );
        }
        else if ( strcmp( argv[i], "-bg" ) == 0 && i+1 < argc )
        {
            sscanf( argv[i+1], "%x", bg );
        }
        else if ( strcmp( argv[i], "-repeat" ) == 0 && i+1 < argc )
        {
            *repeat = atoi( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-maxvalue" ) == 0 && i+1 < argc )
        {
            *maxvalue = atoi( argv[i+1] );
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
    return 1;
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
    if ( ! parse_args( argc, argv, &w, &h, &fg, &bg, &repeat, &maxvalue, source, type, tooltip ) )
        return 0;
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
    session_data session = connect_display();
    window_data wd = create_window( session, w, h, bg, fg, "PRW" );
    xcb_map_window( wd.session.conn, wd.win );
    window_data* ttwd = NULL;
    TextWidget* ttw = NULL;
    if ( tooltip )
    {
        ttwd = (window_data*)malloc(sizeof(window_data));
        *ttwd = create_window( session, 1, 1, 0x777700, 0x777777, NULL );
        ttw = (TextWidget*)malloc( sizeof(TextWidget) );
        *ttw = create_tooltip_widget( tooltip, *ttwd );
        resize_widget( ttw );
        xcb_map_window( wd.session.conn, ttwd->win );
    }

    // show
    xcb_flush(wd.session.conn);

    DerivedWidget widget;
    void (*draw)(Widget*);// the widget specifig draw function
    if ( strcmp( type, "-b" ) == 0 )
    {
        BarWidget* bw = (BarWidget*)&widget;
        draw = draw_barwidget;
        *bw = create_barwidget( source, tooltip, maxvalue, wd );
    }
    else if ( strcmp( type, "-x" ) == 0 )
    {
        TextWidget* tw = (TextWidget*)&widget;
        draw = draw_textwidget;
        *tw = create_textwidget( source, tooltip, wd );
    }
    else if ( strcmp( type, "-r" ) == 0 )
    {
        TrendWidget* tw = (TrendWidget*)&widget;
        draw = draw_trendwidget;
        *tw = create_trendwidget( source, tooltip, maxvalue, wd );
    }
    xcb_generic_event_t* event;
    time_t last_update = time(NULL);
    int update_needed = 0;
    while( 1 ) 
    {
        if ( (event = xcb_poll_for_event(widget.base.wd.session.conn) ) )
        {
            switch( event->response_type & ~0x80 )
            {
                case XCB_EXPOSE:
                    update_needed = 1;
                    break;
                default:
                    break;

            }
            free(event);
            event = NULL;
        }

        time_t t = time(NULL);
        update_needed = update_needed || ( t - last_update >= repeat );
        if ( update_needed )
        {
            last_update = t;
            draw( &widget.base );
            if ( tooltip )
            {
                draw_textwidget( &ttw->base );
            }
            xcb_flush(widget.base.wd.session.conn);
            update_needed = 0;
        }
        nanosleep( &(struct timespec){.tv_nsec = 500000000 }, NULL );
    }
    destroy_widget( &widget.base );
    xcb_disconnect( widget.base.wd.session.conn );
    free( ttw );
    free( ttwd );
    return 0;
}
