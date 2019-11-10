import numpy as np
output_halide_flat = np.loadtxt("/curr/jiajieli/app_halide2heterocl/conv/output_halide.txt")
output_halide = np.transpose(output_halide_flat.reshape((4,32,64,64)), (3,2,1,0)).astype("int32")

# output_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl.npy")
output_heterocl = np.load("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl_generate.npy")
output_heterocl_unroll = np.load("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl_unroll_generate.npy")


# print(output_heterocl)
# print(output_halide)

print("halide and generate: ",np.array_equal(output_halide, output_heterocl))
print("halide and unroll generate: ",np.array_equal(output_halide, output_heterocl_unroll))

# print(output_heterocl.shape)
# print(output_halide.shape)
# print(output_halide[0,0,0,0] == output_heterocl[0,0,0,0])
# print(output_halide[0,0,0,0])
