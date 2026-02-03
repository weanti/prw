#ifndef TOOLTIP_WIDGET_H
#define TOOLTIP_WIDGET_H

#include "xconnection.h"
#include "textwidget.h"

void resize_widget( TextWidget* tw );
// create text widget with static output
TextWidget create_tooltip_widget(   char* text,
                                   window_data* parent );
#endif
