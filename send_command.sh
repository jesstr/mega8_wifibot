#!/bin/sh

PORT=/dev/ttyUSB0
BAUD=38400

# Port configuration
# stty -F $PORT $BAUD

echo -e -n "$COMMAND\n" >> $PORT

# Manual run:
# echo -e -n "TurrLR=520,520\n" >> /dev/ttyUSB0
