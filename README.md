# prw
Programmable Widget for Unix like systems. 

## What is it and how it came to be?
It is a tool that displays an output of a program (script or binary) in a
window.
I prefer lightweight window managers. I'm using JWM and wanted to have that nice
little status widgets that IceWM has, so I implemented this application, which
can be embedded in JWM using the "swallow" tag. (In this case use "FLTK" as
name, becase that is the window class JWM checks.)

## What is it capable of and how to use?
PRW is not as fancy as it sounds. It can (currently) display text or a value
over time (I call it trend).
The output of the "source" application is displayed in the widget. Source
can be am inline shell script, a script file path or a path to a binary
application. In case of text widget it is preferable to produce a single line
output, in case of trend widget the output must be a single number.

Let's see some real life examples:
### Display average load trend, update it in every second:
./prw -r -source "cat /proc/loadavg | awk '{print $1}' 
The 1 minute load average is queried every second and displayed as a thin bar,
old load values are shifted left. Default sizes and colors are used.

### Display wifi connected to:
./prw -x -source "iw wlan0 link | grep SSID | awk '{print $2}'

Full parameter list can be obtained by ./prw -h .

## How to compile
The only library dependency is FLTK. It is implemented using 1.3.X, but
1.4 shall be compatible too.
Other requirements:
* C++11 compliant compiler
* cmake 3.5 or newer
How to compile:
cmake -S <source location> -B <build location> -DFLTK_INCLUDE_DIR=... -DFLTK_LIBRARY_DIR=...
cmake --build <build location>
FLTK_INCLUDE_DIR is likely /usr/include
FLTK_LIBRARY_DIR is likely /usr/lib
Automatic FLTK search may be implemented in the future.
