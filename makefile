CFLAGS!= pkg-config --cflags xcb pango cairo pangocairo
LDFLAGS!= pkg-config --libs xcb pango cairo pangocairo
SOURCES=prw.c barwidget.c trendwidget.c textwidget.c widget.c source.c
OBJECTS=${SOURCES:S/.c/.o/g}

.SUFFIXES: .cpp .o

.cpp.o:
	${CC} ${CFLAGS} -c $<
prw: ${OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJECTS}
