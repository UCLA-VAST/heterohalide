import numpy as np 
# import cv2 
# output_my = cv2.imread("out_halide.png")
# output_hls = cv2.imread("/curr/jiajieli/Halide-HLS/apps/hls_examples/unsharp_hls/out.png")
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)
# print(np.array_equal(output_my, output_hls)) # True
# print(dif(output_my, output_hls)) # 0

def count_dif_elem(a,b):
    return np.size(a) - np.sum(a==b)

# print(count_dif_elem(output_my, output_hls)) # 0



output_heterocl = np.load("output_heterocl.npy")
output_halide = np.loadtxt("output_halide.txt")
output_halide = np.transpose(output_halide.reshape((3, 8, 8)), (2, 1, 0))
print("halide and hcl: ", np.array_equal(output_heterocl, output_halide))
print("dif: halide and hcl: ", dif(output_heterocl, output_halide))

output_heterocl_0 = np.load("output_heterocl_0.npy")
output_heterocl_1 = np.load("output_heterocl_1.npy")
output_heterocl_2 = np.load("output_heterocl_2.npy")
output_heterocl_separate = np.concatenate((np.expand_dims(output_heterocl_0, axis = -1), np.expand_dims(output_heterocl_1, axis = -1), np.expand_dims(output_heterocl_2, axis = -1)), axis = -1)
print("halide and hcl separate(merge): ", np.array_equal(output_heterocl_separate, output_halide)) 
print("dif: halide and hcl separate: ", dif(output_heterocl_separate, output_halide))


# ratio_heterocl = np.load("ratio_heterocl.npy")
# ratio_halide = np.loadtxt("ratio_halide.txt")
# ratio_halide = np.transpose(ratio_halide.reshape((8, 8)), (1, 0))
# print("ratio: halide and hcl: ", np.array_equal(ratio_heterocl, ratio_halide))
# print("ratio dif: halide and hcl: ", dif(ratio_heterocl, ratio_halide))

# gray_heterocl = np.load("gray_heterocl.npy")
# gray_halide = np.loadtxt("gray_halide.txt")
# gray_halide = np.transpose(gray_halide.reshape((16, 16)), (1, 0))
# print("gray: halide and hcl: ", np.array_equal(gray_heterocl, gray_halide))
# print("gray dif: halide and hcl: ", dif(gray_heterocl, gray_halide))


# sum_x_heterocl = np.load("sum_x_heterocl.npy")
# sum_x_halide = np.loadtxt("sum_x_halide.txt")
# sum_x_halide = np.transpose(sum_x_halide.reshape((8, 8)), (1, 0))
# print("sum_x: halide and hcl: ", np.array_equal(sum_x_heterocl, sum_x_halide))
# print("sum_x dif: halide and hcl: ", dif(sum_x_heterocl, sum_x_halide))

# blur_x_heterocl = np.load("blur_x_heterocl.npy")
# blur_x_halide = np.loadtxt("blur_x_halide.txt")
# blur_x_halide = np.transpose(blur_x_halide.reshape((8, 8)), (1, 0))
# print("blur_x: halide and hcl: ", np.array_equal(blur_x_heterocl, blur_x_halide))
# print("blur_x dif: halide and hcl: ", dif(blur_x_heterocl, blur_x_halide))

# sharpen_heterocl = np.load("sharpen_heterocl.npy")
# sharpen_halide = np.loadtxt("sharpen_halide.txt")
# sharpen_halide = np.transpose(sharpen_halide.reshape((8, 8)), (1, 0))
# print("sharpen: halide and hcl: ", np.array_equal(sharpen_heterocl, sharpen_halide))
# print("sharpen dif: halide and hcl: ", dif(sharpen_heterocl, sharpen_halide))
# print("sharp dif elem: ", count_dif_elem(sharpen_heterocl, sharpen_halide))

# out_heterocl_generate = np.load("output_heterocl_generate.npy")
# print("halide and hcl generate: ", np.array_equal(out_heterocl_generate, out_halide))