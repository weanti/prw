#include "textwidget.h"

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include <utility>

TextWidget::TextWidget( int w, int h, int fg, int bg, int repeat, Source source, const std::string& tooltip )
: Widget( w, h, fg, bg, repeat, std::move(source), tooltip )
{
}

void TextWidget::update()
{
    mLabel = execute().c_str();
    redraw();
}

void TextWidget::draw()
{
    Widget::draw();
    fl_color( fg() );
    fl_font( FL_HELVETICA, FL_NORMAL_SIZE );
    fl_draw( mLabel.c_str(), x(), y(), w(), h(), FL_ALIGN_CENTER );
}
