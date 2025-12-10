#ifndef WIDGET_H
#define WIDGET_H

#include "source.h"

#include <FL/Fl_Widget.H>

#include <string>

class Widget : public Fl_Widget
{
    public:
        Widget( int w, int h, int fg, int bg, int repeat, Source source, const std::string& tooltip = "" );
        ~Widget() override = default;

        void draw() override;// Fl_Widget
        int handle(int) override;
        void repeat();

        virtual void update() = 0;
    protected:
        std::string execute();
        int bg() const;
        int fg() const;
    private:
        int mFG;
        int mBG;
        int mRepeat;// interval in seconds
        Source mSource;
};

#endif
