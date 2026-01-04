#!/bin/sh
# Prints master volume level.
# NOTE: output may not be the same on all hardware, so check sndioctl output manually and adapt is necessary.
sndioctl | grep "output.level" | cut -f2 -d'='
