#!/bin/bash
set -e
base_dir="$(realpath "$(dirname "$0")")"
halide_src="${base_dir}/../Halide"
build_dir="${base_dir}/../build"

# add '#define _CODEGEN_HETEROCL_GENERATE_' to the file 'Lower.cpp'
sed -i '1i\#define _CODEGEN_HETEROCL_GENERATE_' "${halide_src}"/src/Lower.cpp
cmake --build "${build_dir}" --target Halide

for app in "${base_dir}"/*/*.cpp; do
  g++ "${app}" -g -std=c++11 \
    -I "${build_dir}"/include \
    -L "${build_dir}"/lib \
    -I "${halide_src}"/tools \
    -l Halide \
    -o "${app/%.cpp/}"
  cd "$(dirname "${app}")"
  HL_DEBUG_CODEGEN=0 LD_LIBRARY_PATH="${build_dir}"/lib "${app/%.cpp/}"
done

# delete '#define _CODEGEN_HETEROCL_GENERATE_' in the file 'Lower.cpp'
sed -i '/#define _CODEGEN_HETEROCL_GENERATE_/d' "${halide_src}"/src/Lower.cpp
cmake --build "${build_dir}" --target Halide
