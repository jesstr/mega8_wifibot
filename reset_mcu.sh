#!/bin/sh

MK=m8
PRG=avr910
BAUD=57600
PORT=/dev/ttyACM0
FLASHFILE=./tmpfile.hex

touch $FLASHFILE > /dev/null 2>&1
sudo avrdude -p $MK -c $PRG -b $BAUD -P $PORT -U signature:r:$FLASHFILE:i > /dev/null 2>&1
sudo rm -rf $FLASHFILE > /dev/null 2>&1

echo "MCU reset performed."

# Manual run:
# sudo avrdude -p m8 -c avr910 -b 57600 -P /dev/ttyACM0 -U flash:w:"./Release/mega8_wifibot.hex":i
