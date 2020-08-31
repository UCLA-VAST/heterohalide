#!/bin/bash
base_dir="$(realpath "$(dirname "$0")")"
halide_src="${base_dir}/../../Halide"
build_dir="${base_dir}/../../build"

# add '#define _CODEGEN_HETEROCL_GENERATE_' to the file 'Lower.cpp'
sed -i '1i\#define _CODEGEN_HETEROCL_GENERATE_' "${halide_src}"/src/Lower.cpp
cmake --build "${build_dir}" --target Halide

g++ "${base_dir}"/blur.cpp -g -std=c++11 \
  -I "${build_dir}"/include \
  -I "${halide_src}"/tools \
  -L "${build_dir}"/lib \
  -l Halide \
  -o "${base_dir}/blur"
cd "${base_dir}"
HL_DEBUG_CODEGEN=0 LD_LIBRARY_PATH="${build_dir}"/lib "${base_dir}"/blur

# delete '#define _CODEGEN_HETEROCL_GENERATE_' in the file 'Lower.cpp'
sed -i '/#define _CODEGEN_HETEROCL_GENERATE_/d' "${halide_src}"/src/Lower.cpp
cmake --build "${build_dir}" --target Halide
