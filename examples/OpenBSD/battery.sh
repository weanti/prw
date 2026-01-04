#!/bin/sh
# NOTE: sysctl query string may be different for other systems.
if [ $# -eq 1 ]
then
    case $1 in
        "-max") 
            MAX=`sysctl -n hw.sensors.acpibat0.watthour4 | awk '{print $1}'`
            echo $MAX
            return;;
        "-h") 
            echo $0 " [-max | -h]"
            echo "Prints remaining battery capacity if no argument is given."
            echo "-max: Prints design (or max) capacity."
            echo "-h: prints this help."
            return;;
        *)
            echo "Unknown option. Invoke with -h for help."
            return;;
    esac
fi
REMAINING=`sysctl -n hw.sensors.acpibat0.watthour3 | awk '{print $1}'`
echo $REMAINING
