import numpy as np 
output_halide_flat = np.loadtxt("/curr/jiajieli/app_halide2heterocl/simple/split/output_halide.txt")
output_halide = np.transpose(output_halide_flat.reshape((10, 8)), (1,0))
output_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/simple/split/output_heterocl.npy")
np.array_equal(output_halide, output_heterocl)