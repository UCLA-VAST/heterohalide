import numpy as np
output_halide_flat = np.loadtxt("/curr/jiajieli/new/conv/output_halide.txt")
output_halide = np.transpose(output_halide_flat.reshape((4,32,64,64)), (3,2,1,0)).astype("int32")

output_heterocl = np.load("/curr/jiajieli/new/conv/output_heterocl.npy")
output_target = np.load("/curr/jiajieli/new/conv/output_target.npy")

print("halide and generate: ", np.array_equal(output_halide, output_heterocl)) # True
print("halide and target: ", np.array_equal(output_halide, output_target)) # True
