#!bin/bash
g++ dilation_exe.cpp dilation_lib.a -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o dilation_exe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/dilation/cpu_clock/dilation_exe