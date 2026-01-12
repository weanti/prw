FLAGS!= pkg-config --cflags xcb pango cairo pangocairo
# due to the cast of structs to "derived" class it is probably a good idea to disable optimizations, otherwise struct members may be reaordered and mess up the expected memory addresses. Dunno, just guessing
CFLAGS+=-O0 ${FLAGS}
LDFLAGS!= pkg-config --libs xcb pango cairo pangocairo
LDFLAGS+=-lm
SOURCES=prw.c barwidget.c trendwidget.c textwidget.c tooltip_textwidget.c widget.c source.c xconnection.c
OBJECTS=${SOURCES:S/.c/.o/g}

.MAIN: prw
.SUFFIXES: .cpp .o
.SILENT: clean

.cpp.o:
	${CC} ${CFLAGS} -c $<
prw: ${OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJECTS}
clean:
	rm prw ${OBJECTS}
