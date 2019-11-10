import numpy as np 
import cv2 
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)
def count_dif_elem(a,b):
    return np.size(a) - np.sum(a==b)

output_my = cv2.imread("out_halide.png")
output_hls = cv2.imread("/curr/jiajieli/Halide-HLS/apps/hls_examples/stereo_hls/out.png")

output_hls_cut = output_hls[4:397+4, 4:629+4, : ]
print("my shape: ", output_my.shape)
print("hls shape: ", output_hls.shape)
# print(np.array_equal(output_my, output_hls)) # True
# print(dif(output_my, output_hls)) # 0
# print(count_dif_elem(output_my, output_hls)) # 0


print(np.array_equal(output_my, output_hls_cut)) # True
print(dif(output_my, output_hls_cut)) # 0
print(count_dif_elem(output_my, output_hls_cut)) # 0


# output_heterocl = np.load("output_heterocl.npy")
# output_halide = np.loadtxt("output_halide.txt")
# output_halide = np.transpose(output_halide.reshape((3, 8, 8)), (2, 1, 0))
# print("halide and hcl: ", np.array_equal(output_heterocl, output_halide))
# print("dif: halide and hcl: ", dif(output_heterocl, output_halide))