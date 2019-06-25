#!/bin/bash
bindir=~/art-frame/build/bin
cd $bindir
echo [starting art-frame from bindir=$PWD]
./art-frame > ../art-frame.log
