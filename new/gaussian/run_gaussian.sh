#!/bin/bash
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/new/gaussian
export HL_DEBUG_CODEGEN=0
g++ gaussian.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o gaussian
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/gaussian/gaussian