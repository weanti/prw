#!/bin/sh
# Prints 1 minute load average.
sysctl -n vm.loadavg | awk '{print $1}'
