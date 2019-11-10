#!/bin/bash
cd ~/Halide
export LLVM_CONFIG=~/llvm8.0/build/bin/llvm-config
make bin/libHalide.so -j50
cd ~/app_stencil/gaussian
rm clock.txt
export HL_DEBUG_CODEGEN=0
g++ gaussian.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o gaussian
for times in 1 2 3 4 5 6 7 8 9 10
do
    LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/gaussian/gaussian cpu
done
python3 clock_cal.py