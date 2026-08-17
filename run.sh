#!/usr/bin/env bash

idf.py build

# locate the port that the device is using
port=$(ls /dev/tty.usb*)
idf.py -p $port flash

idf.py monitor
