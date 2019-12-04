#!/bin/bash
g++ blur_auto_scheduler_generate.cpp ~/Halide/tools/GenGen.cpp -g -std=c++11 -fno-rtti -I ~/Halide/include -L ~/Halide/bin -lHalide -lpthread -ldl -o blur_generate
export LD_LIBRARY_PATH=~/Halide/bin   # For linux
./blur_generate -o . -g auto_schedule_gen -f auto_schedule_false -e static_library,h,schedule target=host auto_schedule=false
./blur_generate -o . -g auto_schedule_gen -f auto_schedule_true -e static_library,h,schedule target=host auto_schedule=true machine_params=32,16777216,40
g++ blur_auto_scheduler_run.cpp -std=c++11 -I ~/Halide/include -I ~/Halide/tools auto_schedule_false.a auto_schedule_true.a -ldl -lpthread -o blur_run
./blur_run
