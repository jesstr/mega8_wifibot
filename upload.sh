#!/bin/sh

MK=m8
PRG=avr910
BAUD=57600
PORT=/dev/ttyACM0
FLASHFILE=./Release/mega8_wifibot.hex

sudo avrdude -p $MK -c $PRG -b $BAUD -P $PORT -U flash:w:$FLASHFILE

# Manual run:
# sudo avrdude -p m8 -c avr910 -b 57600 -P /dev/ttyACM0 -U flash:w:"./Release/mega8_wifibot.hex":i
