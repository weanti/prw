#ifndef DYNAMIC_TEXTWIDGET_H
#define DYNAMIC_TEXTWIDGET_H

#include "widget.h"
#include "xconnection.h"
#include "textwidget.h"

// create text widget with dynamic output, the source of the text is the output of the given program
TextWidget create_dynamic_textwidget(   char* program,
                                        char* tooltip,
                                        window_data xd );
#endif
