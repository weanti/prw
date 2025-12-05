project( prw )
cmake_minimum_required(VERSION 3.5)

file( GLOB SRC prw.cpp source.h source.cpp widget.h widget.cpp textwidget.h textwidget.cpp tredwidget.h trendwidget.cpp )

add_executable( prw ${SRC} )

target_include_directories( prw PRIVATE ${FLTK_INCLUDE_DIR} )
target_link_directories( prw PRIVATE ${FLTK_LIBRARY_DIR} )

target_link_libraries( prw PUBLIC fltk )
