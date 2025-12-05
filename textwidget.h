#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "widget.h"
#include "source.h"

#include <string>

class TextWidget : public Widget
{
    public:
        TextWidget( int w, int h, int fg, int bg, int repeat, Source source, const std::string& tooltip = "" );
        virtual ~TextWidget() =  default;

        void update() override;
        void draw() override; // Fl_Widget
    private:
        std::string mLabel;
};

#endif
