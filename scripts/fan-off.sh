#!/bin/sh

# turn off fan by setting GPIO4 to logical 0.
echo -n "0" > /sys/class/gpio/gpio4/value
