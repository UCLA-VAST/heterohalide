import numpy as np 
input = np.load("/curr/jiajieli/app_halide2heterocl/simple/input.npy")
output_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/simple/output_heterocl.npy")
output_halide = np.loadtxt("/curr/jiajieli/app_halide2heterocl/simple/output_halide.txt")
output_halide = np.transpose(output_halide.reshape((10, 10)), (1, 0))
print(input)
print(output_heterocl)
print(output_halide)
print(np.array_equal(output_heterocl, output_halide))