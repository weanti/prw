#ifndef TRENDWIDGET_H
#define TRENDWIDGET_H

#include "widget.h"
#include "source.h"

#include <string>
#include <vector>

class TrendWidget : public Widget
{
    public:
        TrendWidget( int w, int h, int fg, int bg, int repeat, Source source, double maxvalue, const std::string& tooltip = "" );
        ~TrendWidget() override = default;

        void update() override;
        void draw() override;// Fl_Widget
    private:
        std::vector<double> mTrend;
        double mMaxValue;
};

#endif
