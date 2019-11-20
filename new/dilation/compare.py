import numpy as np 
output_heterocl = np.load("/curr/jiajieli/new/dilation/output_heterocl.npy")
print(output_heterocl.shape) # [w, h]
output_halide = np.loadtxt("/curr/jiajieli/new/dilation/output_halide.txt")
output_halide = np.transpose(output_halide.reshape((480, 640)), (1, 0)) # np.transpose(output_halide.reshape((h, w)), (1, 0))
print(output_halide.shape)
print(np.array_equal(output_halide, output_heterocl)) # True



# input_heterocl = np.load("/curr/jiajieli/new/dilation/input.npy")
# input_halide = np.loadtxt("/curr/jiajieli/new/dilation/input.txt")
# input_halide = np.transpose(input_halide.reshape((482, 642)), (1, 0))