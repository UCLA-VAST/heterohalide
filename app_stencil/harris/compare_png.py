import numpy as np 
# import cv2 
# output_my = cv2.imread("out.png")
# output_hls = cv2.imread("/curr/jiajieli/Halide-HLS/apps/hls_examples/harris_hls/out.png")
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)
# print(np.array_equal(output_my, output_hls))
# print(dif(output_my, output_hls))

def count_dif_elem(a,b):
    return np.size(a) - np.sum(a==b)

# print(count_dif_elem(output_my, output_hls)) # 6 different pixels, total: (2448 - 6) * (3264 - 6)

# output_hcl = cv2.imread("out_hcl.png")
# print(count_dif_elem(output_hcl, output_hls))

output_halide_old = np.loadtxt("output_halide_old.txt")
output_halide_old = np.transpose(output_halide_old.reshape((3258, 2442)), (1, 0)) # reshape: (h, w)

output_halide = np.loadtxt("output_halide.txt")
output_halide = np.transpose(output_halide.reshape((3258, 2442)), (1, 0)) # reshape: (h, w)

output_heterocl = np.load("output_heterocl.npy")
print("output heterocl and halide: ", np.array_equal(output_heterocl, output_halide_old)) #Finaly!!!!!!!!!! True
print(dif(output_halide, output_heterocl))
print("halide and old: ", np.array_equal(output_halide, output_halide_old))
print("dif elem halide and old: ", count_dif_elem(output_halide_old, output_halide))

output_heterocl_optimize = np.load("output_heterocl_optimize.npy")
print("output optimize: ", np.array_equal(output_heterocl_optimize, output_halide)) #Finaly!!!!!!!!!! True

output_heterocl_optimize_remain_hw_output = np.load("output_heterocl_optimize_remain_hw_output.npy")
print("output optimize_remain_hw_output: ", np.array_equal(output_heterocl_optimize_remain_hw_output, output_halide)) #Finaly!!!!!!!!!! True

output_heterocl_new = np.load("output_heterocl_new.npy")
print("hcl new and old: ", np.array_equal(output_heterocl, output_heterocl_new))
print("dif hcl new and old: ", dif(output_heterocl, output_heterocl_new))

print("hcl new and halide new: ", np.array_equal(output_heterocl_new, output_halide))
print("dif hcl new and halide new: ", dif(output_heterocl_new, output_halide))
print("dif elem: ", count_dif_elem(output_heterocl_new, output_halide))


output_halide_new = np.loadtxt("output_halide_new.txt")
output_halide_new = np.transpose(output_halide_new.reshape((3258, 2442)), (1, 0)) # reshape: (h, w)

cim_halide_new = np.loadtxt("cim_halide_new.txt")
cim_halide_new = np.transpose(cim_halide_new.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)
cim_heterocl_new = np.load("cim_heterocl_new.npy")

print("cim new: ", np.array_equal(cim_heterocl_new, cim_halide_new))
print("dif cim new: ", dif(cim_heterocl_new, cim_halide_new))

cim_halide = np.loadtxt("cim_halide.txt")
cim_halide = np.transpose(cim_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

grad_gx_halide = np.loadtxt("grad_gx_halide.txt")
grad_gx_halide = np.transpose(grad_gx_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

grad_gy_halide = np.loadtxt("grad_gy_halide.txt")
grad_gy_halide = np.transpose(grad_gy_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

grad_gxy_halide = np.loadtxt("grad_gxy_halide.txt")
grad_gxy_halide = np.transpose(grad_gxy_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

trace_heterocl = np.load("trace_heterocl.npy")