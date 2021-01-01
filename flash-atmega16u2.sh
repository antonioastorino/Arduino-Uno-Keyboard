#!/bin/bash
set -e

if [ "$1" == "" ]; then
	echo "Please specify firmware"
	exit
elif [ -f "$1" ]; then
	FIRMWARE="$1"
else
	echo "Error: '$1' does not exist"
	exit
fi

dfu-programmer atmega16u2 erase --force
dfu-programmer atmega16u2 flash $FIRMWARE
dfu-programmer atmega16u2 reset

