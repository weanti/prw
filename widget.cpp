#include "widget.h"

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include <utility>

// TODO: tooltip handling

void update( void* arg )
{
    ((Widget*)arg)->update();
    ((Widget*)arg)->repeat();
}

Widget::Widget( int w, int h, int fg, int bg, int repeat, Source source, const std::string& tooltip )
: Fl_Widget( 0, 0, w, h )
, mFG( fg )
, mBG( bg )
, mRepeat( repeat )
, mSource( std::move(source) )
{
    copy_tooltip( tooltip.c_str() );
    box(FL_FLAT_BOX);
    color( FL_GRAY );
    // add timer
    Fl::add_timeout( mRepeat, ::update, this ); 
}

void Widget::draw()
{
    // draws background box
    fl_draw_box( FL_FLAT_BOX, x(), y(), w(), h(), mBG );
}

void Widget::repeat()
{
    Fl::repeat_timeout( mRepeat, ::update, this );
}

std::string Widget::execute()
{
    return mSource.execute();
}

int Widget::bg() const
{
    return mBG;
}
int Widget::fg() const
{
    return mFG;
}
