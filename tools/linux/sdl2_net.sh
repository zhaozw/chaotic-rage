#!/bin/bash


cd working

if [ ! -d "SDL2_net-2.0.0" ]; then
	if [ ! -f "SDL2_net-2.0.0.tar.gz" ]; then
		wget http://www.libsdl.org/projects/SDL_net/release/SDL2_net-2.0.0.tar.gz
	fi
	tar -xzf SDL2_net-2.0.0.tar.gz || exit 1
fi

cd SDL2_net-2.0.0

./configure	--disable-gui || exit 1

make || exit 1

sudo make install || exit 1

sudo ldconfig || exit 1
