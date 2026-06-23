#!/bin/bash

# compile and run
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..
./build/FPSGame
