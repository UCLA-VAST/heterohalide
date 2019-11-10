#!/bin/bash
host=/curr/jiajieli/Halide/apps/stencil_chain
dir=/curr/jiajieli/app_halide2heterocl/stencil
python3 ${dir}/input_generator.py
cd ${host}
make
bin/host/process
python3 ${dir}/stencil_chain.py
python3 ${dir}/compare.py