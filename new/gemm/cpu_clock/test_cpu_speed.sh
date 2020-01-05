#!bin/bash
g++ gemm_exe.cpp gemm_lib.a -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o gemm_exe
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/new/gemm/cpu_clock/gemm_exe