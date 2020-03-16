#!/bin/bash

# Generate Intel OpenCL code using the latest SODA
python3.6 /home/blaok/git/soda/src/sodac --iocl-kernel blur_intel_soda.cl blur_gen.soda
export AOCL_BSP=s10mx_ref

# compile the Intel FPGA OpenCL code: obtain the HLS report
aoc -rtl blur_intel_soda.cl -I /opt/tools/intel/intelFPGA_pro/19.4/hld/include/kernel_headers