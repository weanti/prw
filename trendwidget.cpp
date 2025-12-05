#include "trendwidget.h"

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include <algorithm>
#include <cstdlib>
#include <utility>

TrendWidget::TrendWidget(int w, int h, int fg, int bg, int repeat, Source source, double maxvalue, const std::string& tooltip )
    : Widget( w, h, fg, bg, repeat, std::move(source), tooltip )
    , mMaxValue(maxvalue)
{
    mTrend.resize( w );
}

void TrendWidget::update()
{
    for ( int i = 0; i < mTrend.size()-1; i++ )
    {
        mTrend[i] = mTrend[i+1];
    }
    double value = atof( execute().c_str() );
    // scale this value to [0, h()] interval using mMax value
    mTrend[ mTrend.size()-1 ] = std::min( (double)h(), value / mMaxValue * h() );
    redraw();
}

void TrendWidget::draw()
{
    Widget::draw();
    fl_color( fg() );
    fl_line_style( FL_SOLID, 1, nullptr );
    int x = 0;
    for ( const double value : mTrend )
    {
        // draw from bottom to top
        fl_line(x, h(), x, h()-value );
        x++;
    }
}
