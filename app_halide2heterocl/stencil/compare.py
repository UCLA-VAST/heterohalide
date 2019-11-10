import numpy as np 

out_halide_flat = np.loadtxt("/curr/jiajieli/app_halide2heterocl/stencil/output_halide.txt")
out_halide = np.transpose(out_halide_flat.reshape((2548, 1524)), (1, 0))

out_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/stencil/output_heterocl_generate.npy")
out_heterocl_unroll = np.load("/curr/jiajieli/app_halide2heterocl/stencil/output_heterocl_unroll_generate.npy")



print("halide and generate: ", np.array_equal(out_heterocl, out_halide))
print("halide and unroll generate: ", np.array_equal(out_heterocl_unroll, out_halide))