#!/bin/bash
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/app_stencil/unsharp
export HL_DEBUG_CODEGEN=0
g++ unsharp_new.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o unsharp_new
# LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/unsharp/unsharp /curr/jiajieli/Halide-HLS/apps/images/benchmark_8mp_rgb.png
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/unsharp/unsharp_new

# g++ unsharp_separate.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o unsharp_separate
# # LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/unsharp_separate/unsharp_separate /curr/jiajieli/Halide-HLS/apps/images/benchmark_8mp_rgb.png
# LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/unsharp/unsharp_separate