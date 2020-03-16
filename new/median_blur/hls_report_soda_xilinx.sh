#!/bin/bash
# U250 board
# ~blaok/git/soda-internal/src/sodac --xocl-platform /opt/xilinx/platforms/xilinx_u250_xdma_201830_2 --xocl-hw-xo xofile_new.xo median_blur_new.soda --recursion-limit 3000 --temporal-cse no
# ~blaok/git/soda-internal/src/report-xo-util <xofile_new.xo >median_blur_new_size_6478_unroll_32.rpt

# F1 board
~blaok/git/soda-internal/src/sodac --xocl-platform /opt/tools/xilinx/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0 --xocl-hw-xo xofile.xo median_blur_new.soda --recursion-limit 3000 --temporal-cse no
echo xofile generated
~blaok/git/soda-internal/src/report-xo-util <xofile.xo >f1_median_blur_new_size_6478_unroll_32.rpt
echo report generated