#!/bin/bash
python /curr/jiajieli/app_halide2heterocl/conv/input_generator.py
g++ conv_layer.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o conv_layer
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/conv/conv_layer
# python /curr/jiajieli/app_halide2heterocl/conv/conv_layer.py
python /curr/jiajieli/app_halide2heterocl/conv/conv_target.py
python /curr/jiajieli/app_halide2heterocl/conv/compare.py