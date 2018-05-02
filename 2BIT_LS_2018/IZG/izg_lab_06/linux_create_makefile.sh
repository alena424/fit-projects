#!/bin/sh

cd sdl2
mkdir linux
cd linux
wget https://www.libsdl.org/release/SDL2-2.0.8.tar.gz
mkdir install
tar xf SDL2-2.0.8.tar.gz
cd SDL2-2.0.8
mkdir build
cd build
cmake -D CMAKE_INSTALL_PREFIX=../../install ..
make -j4 install
cd ../../../../

rootdir=`pwd`
cd build
cmake -D SDL2_DIR=$rootdir/sdl2/linux/install/lib/cmake/SDL2 ..

