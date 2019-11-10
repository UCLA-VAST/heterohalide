import numpy as np 

output_halide_flat = np.loadtxt("/curr/jiajieli/new/stencil_chain/output_halide.txt")
output_halide = np.transpose(output_halide_flat.reshape((2548, 1524)), (1, 0))

output_heterocl = np.load("/curr/jiajieli/new/stencil_chain/output_heterocl.npy")
output_target = np.load("/curr/jiajieli/new/stencil_chain/output_target.npy")



print("halide and generate: ", np.array_equal(output_heterocl, output_halide)) # True
print("halide and target: ", np.array_equal(output_halide, output_target)) # True
