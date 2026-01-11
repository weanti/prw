#ifndef STATIC_TEXTWIDGET_H
#define STATIC_TEXTWIDGET_H

#include "widget.h"
#include "xconnection.h"
#include "textwidget.h"

// create text widget with static output
TextWidget create_static_textwidget( char* text,
                                     char* tooltip,
                                     window_data xd );
#endif
