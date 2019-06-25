#!/bin/sh

# turn on fan by setting GPIO4 to logical 1.
echo -n "1" > /sys/class/gpio/gpio4/value
