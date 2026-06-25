#!/bin/bash

set -euo pipefail # make sure program exits on compile/linker error

# compile and run
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..
clear
./build/FPSGame
