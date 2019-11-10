#!/bin/bash
# cd ~/Halide
# export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
# make bin/libHalide.so
cd ~/app_halide2heterocl/simple/split
export HL_DEBUG_CODEGEN=1
g++ simple_split.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o simple_split
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/simple/split/simple_split