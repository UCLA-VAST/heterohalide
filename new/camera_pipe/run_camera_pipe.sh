#!/bin/bash
sed -i '1i\#define _CODEGEN_HETEROCL_GENERATE_' /curr/jiajieli/Halide/src/Lower.cpp # add '#define _CODEGEN_HETEROCL_GENERATE_' to the file 'Lower.cpp'
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/app_stencil/camera_pipe
export HL_DEBUG_CODEGEN=0
g++ camera_pipe.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o camera_pipe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/camera_pipe/camera_pipe /curr/jiajieli/Halide-HLS/apps/images/bayer_raw.png #gray.png
# LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/camera_pipe/camera_pipe

sed -i '/#define _CODEGEN_HETEROCL_GENERATE_/d' /curr/jiajieli/Halide/src/Lower.cpp # delete '#define _CODEGEN_HETEROCL_GENERATE' in the file 'Lower.cpp'