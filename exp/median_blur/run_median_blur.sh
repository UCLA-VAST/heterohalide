#!/bin/bash
cd ~/Halide
sed -i '1i\#define _CODEGEN_HETEROCL_GENERATE_' /curr/jiajieli/Halide/src/Lower.cpp # add '#define _CODEGEN_HETEROCL_GENERATE_' to the file 'Lower.cpp'
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/new/median_blur
export HL_DEBUG_CODEGEN=0
g++ median_blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o median_blur
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/median_blur/median_blur
sed -i '/#define _CODEGEN_HETEROCL_GENERATE_/d' /curr/jiajieli/Halide/src/Lower.cpp # delete '#define _CODEGEN_HETEROCL_GENERATE' in the file 'Lower.cpp'
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50