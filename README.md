# prw
Programmable Widget for Unix like systems using X11. 

## What is it and how it came to be?
It is a tool that displays an output of a program (script or binary) in a
window.
I prefer lightweight window managers. I'm using JWM and wanted to have that nice
little status widgets that IceWM has, so I implemented this application, which
can be embedded in JWM using the "swallow" tag. (In this case use "PRW" as
name, becase that is the window class JWM checks.)

## What is it capable of and how to use?
PRW is not as fancy as it sounds. It can (currently) display text, value as a bar
or a value over time (I call it trend).
The output of the "source" application is displayed in the widget. Source
can be an inline shell script, a script file path or a path to a binary
application.
In case of text widget it is preferable to produce a single line
output. In case of bar widget and trend widget the output must be a single number.

Let's see some real life examples:
### Display average load trend, update it in every second:
./prw -r -source "cat /proc/loadavg | awk '{print $1}'"

The 1 minute load average is queried every second and displayed as a thin bar,
old load values are shifted left. Default sizes and colors are used.

### Display wifi connected to:
./prw -x -source "iw wlan0 link | grep SSID | awk '{print $2}'"

### Display volume level (in OpenBSD)
./prw -b -source "sndioctl | grep 'output.level' | cut -f2 -d'='" -w 8

Full parameter list can be obtained by ./prw -h .

## How to compile
Library dependencies: xcb, pango, cairo, pangocairo.

Other requirements:
* C compiler
* pkg-config
  
How to compile:
make
extra compile flags (like debug symbols "-g") can be appened to the default flags by defining CFLAGS environment variable. By default no compile time optimization is enabled and probably that should be left that way (see comment in the makefile(s)).
