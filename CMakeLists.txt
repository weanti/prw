project( prw )
cmake_minimum_required(VERSION 3.5)

file( GLOB SRC prw.c barwidget.h barwidget.c source.h source.c widget.h widget.c textwidget.h textwidget.c trendwidget.h trendwidget.c )

add_executable( prw ${SRC} )

target_include_directories( prw PRIVATE ${XCB_INCLUDE_DIR} )
target_link_directories( prw PRIVATE ${XCB_LIBRARY_DIR} )

target_link_libraries( prw PRIVATE xcb m)
