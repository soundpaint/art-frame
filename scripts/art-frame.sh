#!/bin/bash
bindir="$(cd -P "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
echo [found bindir=$bindir]
cd $bindir
echo [starting art-frame from bindir=$PWD]
./art-frame > ../art-frame.log
