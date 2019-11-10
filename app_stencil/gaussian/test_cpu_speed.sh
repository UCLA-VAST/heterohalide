#!bin/bash
g++ gaussian_exe.cpp gaussian_lib.a -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o gaussian_exe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_stencil/gaussian/gaussian_exe