import numpy as np 
output_heterocl = np.load("/curr/jiajieli/new/blur/output_heterocl.npy")
print(output_heterocl.shape) # [w, h]
output_halide = np.loadtxt("/curr/jiajieli/new/blur/output_halide.txt")
output_halide = np.transpose(output_halide.reshape((480, 640)), (1, 0)) # np.transpose(output_halide.reshape((h, w)), (1, 0))
print(output_halide.shape)
print(np.array_equal(output_halide, output_heterocl)) # True

output_target = np.load("/curr/jiajieli/new/blur/output_target.npy")
print(output_target.shape)
print(np.array_equal(output_halide, output_target))