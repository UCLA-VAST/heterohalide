#!bin/bash
g++ median_blur_exe.cpp median_blur_lib.a -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o median_blur_exe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/median_blur/cpu_clock/median_blur_exe