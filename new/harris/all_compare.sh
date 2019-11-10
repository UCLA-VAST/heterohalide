#!/bin/bash
python3 input_generator.py
echo input generated
python3 harris_generate.py
echo generate output_heterocl
bash make_harris.sh
echo generate output_halide
python3 compare.py
echo compare result