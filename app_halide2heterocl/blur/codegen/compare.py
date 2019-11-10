import numpy as np
out_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/blur/codegen/output_heterocl_generate.npy")
out_heterocl_unroll = np.load("/curr/jiajieli/app_halide2heterocl/blur/codegen/output_heterocl_unroll_generate.npy")
out_halide = np.loadtxt("/curr/jiajieli/app_halide2heterocl/blur/output_halide.txt")
out_halide = np.transpose(out_halide.reshape((480, 640)), (1, 0))
print("halide and generate: ", np.array_equal(out_heterocl, out_halide))
print("halide and generate: ", np.array_equal(out_heterocl_unroll, out_halide))
