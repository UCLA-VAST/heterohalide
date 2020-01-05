#!bin/bash
g++ sobel_exe.cpp sobel_lib.a -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o sobel_exe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/sobel/cpu_clock/sobel_exe