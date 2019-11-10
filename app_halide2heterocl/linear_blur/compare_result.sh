#!/bin/bash
# root='/curr/jiajieli/Halide/cmake_build/bin'
# dir='/curr/jiajieli/app_halide2heterocl/linear_blur'
# ${root}/run_linear_blur 1 ${dir}/rgb.png ${dir}/rgb_linear_blur_Halide.png
# python3 ${dir}/linear_blur.py
# python3 compare.py 


python /curr/jiajieli/app_halide2heterocl/linear_blur/input_generator.py
g++ linear_blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o linear_blur
LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/linear_blur/linear_blur
python /curr/jiajieli/app_halide2heterocl/linear_blur/linear_blur.py
python /curr/jiajieli/app_halide2heterocl/linear_blur/linear_blur_target.py
python /curr/jiajieli/app_halide2heterocl/linear_blur/compare.py