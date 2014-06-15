#!/bin/bash

# this ugly script was written by lk16 on june 15th, 2014
# it compiles the squared source code into a windows binary file (.exe)

MXE_ROOT=/opt/mxe
CROSS=i686-pc-mingw32

GXX=$MXE_ROOT/usr/bin/$CROSS-g++
PKG_CONFIG=$MXE_ROOT/usr/bin/$CROSS-pkg-config


FLAGS="-I .. -std=c++0x -Wall -DNDEBUG -Wextra -O3 -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 

LIBS=`$PKG_CONFIG --libs --cflags gtkmm-2.4 glibmm-2.4 gdk-pixbuf-2.0`

$GXX -I ./src $(find ./src -name *.cpp) $FLAGS $LIBS -o squared.exe



