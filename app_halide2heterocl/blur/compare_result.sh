#!/bin/bash
python3 /curr/jiajieli/app_halide2heterocl/blur/input_generator.py
python3 /curr/jiajieli/app_halide2heterocl/blur/blur.py
g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/blur
python3 /curr/jiajieli/app_halide2heterocl/blur/compare.py