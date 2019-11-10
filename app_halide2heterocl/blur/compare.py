import numpy as np
out_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/blur/output_heterocl.npy")
out_halide = np.loadtxt("/curr/jiajieli/app_halide2heterocl/blur/output_halide.txt")
out_halide = np.transpose(out_halide.reshape((480, 640)), (1, 0))
print(np.array_equal(out_heterocl, out_halide))
