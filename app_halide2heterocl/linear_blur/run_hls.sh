#!/bin/bash
# source with-sdaccel
# # platform=xilinx_u200_qdma_201910_1
# platform=xilinx_u250_xdma_201830_2
# kernel_name=default_function
# app=linear_blur
# ~blaok/git/soda-internal/src/sodac linear_blur.soda --xocl-kernel ${app}_kernel.cpp --recursion-limit 3000 --temporal-cse no

# xocc -t hw -f ${platform} --kernel ${kernel_name}_kernel --xp prop:kernel.${kernel_name}_kernel.kernel_flags="-std=c++0x" -c ${app}_kernel.cpp -o ${app}.hw.xo -s
# ~blaok/git/soda-internal/src/sodac --xocl-platform ~blaok/git/soda/zynq7000 --xocl-hw-xo xofile_real.xo linear_blur.soda --recursion-limit 3000 --temporal-cse no

# ~blaok/git/soda-internal/src/sodac --xocl-platform /opt/xilinx/platforms/xilinx_u250_xdma_201830_2 --xocl-hw-xo xofile.xo linear_blur.soda --recursion-limit 3000 --temporal-cse no
# echo xofile generated
# ~blaok/git/soda-internal/src/report-xo-util <xofile.xo >report_dim3_size_768_1280_unroll_8.rpt
# echo report generated

~blaok/git/soda-internal/src/sodac --xocl-platform /opt/tools/xilinx/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0 --xocl-hw-xo xofile.xo linear_blur.soda --recursion-limit 3000 --temporal-cse no
echo xofile generated
~blaok/git/soda-internal/src/report-xo-util <xofile.xo >f1_dim3_size_768_1280_unroll_8.rpt
echo report generated
