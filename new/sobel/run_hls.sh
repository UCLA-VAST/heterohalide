#!/bin/bash
# U250 board
# ~blaok/git/soda-internal/src/sodac --xocl-platform /opt/xilinx/platforms/xilinx_u250_xdma_201830_2 --xocl-hw-xo xofile.xo sobel.soda --recursion-limit 3000 --temporal-cse no
# ~blaok/git/soda-internal/src/report-xo-util <xofile.xo >sobel_size_6478_unroll_16.rpt

# F1 board
~blaok/git/soda-internal/src/sodac --xocl-platform /opt/tools/xilinx/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0 --xocl-hw-xo xofile.xo sobel.soda --recursion-limit 3000 --temporal-cse no
echo xofile generated
~blaok/git/soda-internal/src/report-xo-util <xofile.xo >f1_sobel_size_6478_unroll_16.rpt
echo report generated