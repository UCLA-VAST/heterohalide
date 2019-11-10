import numpy as np 
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)
# print(np.array_equal(output_my, output_hls)) # True
# print(dif(output_my, output_hls)) # 0

# def count_dif_elem(a,b):
#     return np.size(a) - np.sum(a==b)

# print(count_dif_elem(output_my, output_hls)) # 0

# out_my = np.loadtxt("/curr/jiajieli/app_stencil/fanout/output_halide.txt")
# out_halide_hls = np.loadtxt("/curr/jiajieli/Halide-HLS/apps/hls_examples/fanout_hls/output_hls.txt")

# print(np.array_equal(out_my, out_halide_hls))




out_heterocl = np.load("output_heterocl.npy")
out_halide = np.loadtxt("output_halide.txt")
print("halide and hcl target: ", np.array_equal(out_heterocl, out_halide))
print("dif: halide and hcl target: ", dif(out_heterocl, out_halide))

# out_heterocl_generate = np.load("output_heterocl_generate.npy")
# print("halide and hcl generate: ", np.array_equal(out_heterocl_generate, out_halide))