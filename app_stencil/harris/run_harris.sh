#!/bin/bash
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/app_stencil/harris
export HL_DEBUG_CODEGEN=0
g++ harris.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o harris
# LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/harris/harris /curr/jiajieli/Halide-HLS/apps/images/benchmark_8mp_gray.png
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/harris/harris