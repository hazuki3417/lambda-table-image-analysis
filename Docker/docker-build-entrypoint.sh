#! /usr/bin/env bash

if [ $# != 1 ]; then
  echo There is no argument.
  exit 1
fi

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/install
make
make $1
