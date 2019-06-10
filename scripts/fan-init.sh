#!/bin/sh
echo -n "4" > /sys/class/gpio/export
sleep 0.1
echo -n "out" > /sys/class/gpio/gpio4/direction
sleep 0.1
