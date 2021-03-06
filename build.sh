#!/bin/sh
cd ctemplate
sudo libtoolize && sudo aclocal && sudo autoheader && sudo autoconf && sudo automake --add-missing
sh configure
cd ..
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=relwithdebinfo ..
make -j
cd ..
