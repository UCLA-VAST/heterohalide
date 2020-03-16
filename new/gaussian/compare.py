import numpy as np 
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)
# def count_dif_elem(a,b):
#     return np.size(a) - np.sum(a==b)

out_heterocl = np.load("output_heterocl.npy")
out_halide = np.loadtxt("output_halide.txt")
out_halide = np.transpose(out_halide.reshape((600 - 8, 400 - 8)), (1, 0)) # np.transpose(output_halide.reshape((h, w)), (1, 0))
print("halide and hcl: ", np.array_equal(out_heterocl, out_halide)) # True
print("dif: halide and hcl: ", dif(out_heterocl, out_halide)) # 0