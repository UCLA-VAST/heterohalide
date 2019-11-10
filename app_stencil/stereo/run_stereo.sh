#!/bin/bash
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/app_stencil/stereo
export HL_DEBUG_CODEGEN=3
g++ stereo.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o stereo
# LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/stereo/stereo /curr/jiajieli/Halide-HLS/apps/images/left0224.png /curr/jiajieli/Halide-HLS/apps/images/left-remap.png /curr/jiajieli/Halide-HLS/apps/images/right0224.png /curr/jiajieli/Halide-HLS/apps/images/right-remap.png
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/stereo/stereo