#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

#include <cstdio>
#include <cstdlib>
#include <memory>

#include "barwidget.h"
#include "textwidget.h"
#include "trendwidget.h"

const int DEFAULT_WIDTH = 32;
const int DEFAULT_HEIGHT = 32;
const int DEFAULT_BG = 0x77777700;
const int DEFAULT_FG = 0x0000BB00;
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
                -fg: foreground color in hex value. Text color or bar color. Format is the following: 0xRRGGBB00. Default is %x\n\
                -bg: background color in hex value. Format is the following: 0xRRGGBB00. Default is %x\n\
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
    std::string source;
    std::string type;
    std::string tooltip;
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
            source = std::string( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-tooltip" ) == 0 && i+1 < argc )
        {
            tooltip = std::string( argv[i+1] );
        }
        else if ( strcmp( argv[i], "-b" ) == 0 || strcmp( argv[i], "-x" ) == 0 || strcmp( argv[i], "-r" ) == 0 ) 
        {
            type = std::string( argv[i] );
        }
        else if ( strcmp( argv[i], "-h" ) == 0 )
        {
            usage(argv[0]);
            return 0;
        }
    } 
    if ( source.empty() )
    {
        puts("-source is mandatory");
        usage(argv[0]);
        exit(1);
    }
    if ( type.empty() )
    {
        puts("-type is mandatory");
        usage(argv[0]);
        exit(1);
    }
    Fl_Window wnd( w, h );
    wnd.box( FL_FLAT_BOX );
    std::unique_ptr<Widget> widget;
    if ( type == "-b" )
    {
        widget = std::make_unique<BarWidget>( w, h, fg, bg, repeat, std::move( Source(source) ), maxvalue, tooltip );
    } else if ( type == "-x" )
    {
        widget = std::make_unique<TextWidget>( w, h, fg, bg, repeat, std::move( Source(source) ), tooltip );
    }
    else if ( type == "-r" )
    {
        widget = std::make_unique<TrendWidget>( w, h, fg, bg, repeat, std::move( Source(source) ), maxvalue, tooltip );
    }
    wnd.end();
    wnd.show();
    return Fl::run();
}
