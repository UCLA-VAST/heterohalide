#!/bin/bash

# Generate Intel OpenCL code using the latest SODA
python3.6 /curr/jiajieli/soda/src/sodac --iocl-kernel stencil_intel_soda.cl stencil_gen.soda --recursion-limit 5000
# export AOCL_BSP=s10mx_ref

# compile the Intel FPGA OpenCL code: obtain the HLS report
aoc -rtl stencil_intel_soda.cl -I /opt/tools/intel/intelFPGA_pro/19.4/hld/include/kernel_headers