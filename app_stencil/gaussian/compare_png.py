import numpy as np 
# import cv2 
# output_my = cv2.imread("out_halide.png")
# output_hls = cv2.imread("/curr/jiajieli/Halide-HLS/apps/hls_examples/gaussian_hls/out.png")
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)
# print(np.array_equal(output_my, output_hls)) # True
# print(dif(output_my, output_hls)) # 0

# def count_dif_elem(a,b):
#     return np.size(a) - np.sum(a==b)

# print(count_dif_elem(output_my, output_hls)) # 0



out_heterocl = np.load("output_heterocl.npy")
out_halide = np.loadtxt("output_halide.txt")
out_halide = np.transpose(out_halide.reshape((4, 4)), (1, 0))
print("halide and hcl target: ", np.array_equal(out_heterocl, out_halide))
print("dif: halide and hcl target: ", dif(out_heterocl, out_halide))

out_heterocl_ori = np.load("output_heterocl_ori.npy")
print("halide and hcl ori: ", np.array_equal(out_heterocl_ori, out_halide))