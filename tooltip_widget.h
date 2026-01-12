#ifndef TOOLTIP_WIDGET_H
#define TOOLTIP_WIDGET_H

#include "widget.h"
#include "xconnection.h"
#include "textwidget.h"

// create text widget with static output
TextWidget create_tooltip_textwidget(   char* text,
                                        window_data xd );
#endif
