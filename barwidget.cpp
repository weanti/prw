#include "barwidget.h"

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include <algorithm>
#include <cstdlib>
#include <utility>

BarWidget::BarWidget(int w, int h, int fg, int bg, int repeat, Source source, double maxvalue, const std::string& tooltip )
    : Widget( w, h, fg, bg, repeat, std::move(source), tooltip )
    , mMaxValue(maxvalue)
    , mValue{0.0}
{
}

void BarWidget::update()
{
    std::string s = execute();
    double value = atof(s.c_str() );
    // scale this value to [0, h()] interval using mMax value
    mValue = std::min( (double)h(), value / mMaxValue * h() );
    redraw();
}

void BarWidget::draw()
{
    Widget::draw();
    fl_color( fg() );
    // draw from bottom to top
    fl_rectf( 0, h()-mValue, w(), mValue );
}
