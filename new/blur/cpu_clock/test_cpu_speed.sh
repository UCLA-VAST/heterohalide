#!bin/bash
g++ blur_exe.cpp blur_lib.a -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur_exe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/blur/cpu_clock/blur_exe