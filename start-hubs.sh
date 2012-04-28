#!/bin/bash

cd `dirname "$0"`

killall tcphub
killall canbridge
sudo killall cangateway

mkdir build
cd build
cmake ..
make
sudo make install

sleep 1

tcphub 7771 &
tcphub 7772 &

sleep 1

LD_LIBRARY_PATH=/usr/local/lib/
canbridge &

sudo cangateway -H localhost &
