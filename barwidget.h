#ifndef BARWIDGET_H
#define BARWIDGET_H

#include "widget.h"
#include "source.h"

#include <string>

class BarWidget : public Widget
{
    public:
        BarWidget( int w, int h, int fg, int bg, int repeat, Source source, double maxvalue, const std::string& tooltip = "" );
        ~BarWidget() override = default;

        void update() override;
        void draw() override;// Fl_Widget
    private:
        double mMaxValue;
        double mValue;
};

#endif
