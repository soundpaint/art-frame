#!/bin/sh

# create file node /sys/class/gpio/gpio4
echo -n "4" > /sys/class/gpio/export
sleep 0.1

# configure gpio4 as output
echo -n "out" > /sys/class/gpio/gpio4/direction
sleep 0.1
