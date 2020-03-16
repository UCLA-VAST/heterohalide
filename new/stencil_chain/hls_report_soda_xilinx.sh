# ~blaok/git/soda-internal/src/sodac --xocl-platform /opt/xilinx/platforms/xilinx_u250_xdma_201830_2 --xocl-hw-xo xofile.xo stencil_gen.soda --recursion-limit 5000 --temporal-cse no
# echo xofile generated
# ~blaok/git/soda-internal/src/report-xo-util <xofile.xo >report_size_1536_unroll_16.rpt
# echo report generated

~blaok/git/soda-internal/src/sodac --xocl-platform /opt/tools/xilinx/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0 --xocl-hw-xo xofile.xo stencil_gen.soda --recursion-limit 5000 --temporal-cse no
echo xofile generated
~blaok/git/soda-internal/src/report-xo-util <xofile.xo >report_size_1536_unroll_16.rpt
echo report generated