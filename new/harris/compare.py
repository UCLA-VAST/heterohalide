import numpy as np 
def dif(a,b):
    return np.sum(np.abs(a - b)) / np.size(a)

def count_dif_elem(a,b):
    return np.size(a) - np.sum(a==b)

output_halide = np.loadtxt("output_halide.txt")
output_halide = np.transpose(output_halide.reshape((3264 - 6, 2448 - 6)), (1, 0)) 

output_heterocl = np.load("output_heterocl.npy")

print("output heterocl and halide: ", np.array_equal(output_heterocl, output_halide)) 
print(dif(output_halide, output_heterocl)) # 0.96307
print(count_dif_elem(output_halide, output_heterocl)) # 30048

input = np.load("input.npy")


grad_gy_halide = np.loadtxt("grad_gy_halide.txt")
grad_gy_halide = np.transpose(grad_gy_halide.reshape((3264 - 4, 2448 - 4)), (1, 0)) 

grad_gy_heterocl = np.load("grad_gy_heterocl.npy")

print("grad_gy heterocl and halide: ", np.array_equal(grad_gy_heterocl, grad_gy_halide)) # True
print(dif(grad_gy_halide, grad_gy_heterocl)) # 0
print(count_dif_elem(grad_gy_halide, grad_gy_heterocl)) # 0

# Debug!!!!!!!
# Input ok
# grad_x ok
# grad_gx ok
# cim wrong





# print("halide and old: ", np.array_equal(output_halide, output_halide_old))
# print("dif elem halide and old: ", count_dif_elem(output_halide_old, output_halide))

# output_heterocl_optimize = np.load("output_heterocl_optimize.npy")
# print("output optimize: ", np.array_equal(output_heterocl_optimize, output_halide)) #Finaly!!!!!!!!!! True

# output_heterocl_optimize_remain_hw_output = np.load("output_heterocl_optimize_remain_hw_output.npy")
# print("output optimize_remain_hw_output: ", np.array_equal(output_heterocl_optimize_remain_hw_output, output_halide)) #Finaly!!!!!!!!!! True

# output_heterocl_new = np.load("output_heterocl_new.npy")
# print("hcl new and old: ", np.array_equal(output_heterocl, output_heterocl_new))
# print("dif hcl new and old: ", dif(output_heterocl, output_heterocl_new))

# print("hcl new and halide new: ", np.array_equal(output_heterocl_new, output_halide))
# print("dif hcl new and halide new: ", dif(output_heterocl_new, output_halide))
# print("dif elem: ", count_dif_elem(output_heterocl_new, output_halide))


# output_halide_new = np.loadtxt("output_halide_new.txt")
# output_halide_new = np.transpose(output_halide_new.reshape((3258, 2442)), (1, 0)) # reshape: (h, w)

# cim_halide_new = np.loadtxt("cim_halide_new.txt")
# cim_halide_new = np.transpose(cim_halide_new.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)
# cim_heterocl_new = np.load("cim_heterocl_new.npy")

# print("cim new: ", np.array_equal(cim_heterocl_new, cim_halide_new))
# print("dif cim new: ", dif(cim_heterocl_new, cim_halide_new))

# cim_halide = np.loadtxt("cim_halide.txt")
# cim_halide = np.transpose(cim_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

# grad_gx_halide = np.loadtxt("grad_gx_halide.txt")
# grad_gx_halide = np.transpose(grad_gx_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

# grad_gy_halide = np.loadtxt("grad_gy_halide.txt")
# grad_gy_halide = np.transpose(grad_gy_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

# grad_gxy_halide = np.loadtxt("grad_gxy_halide.txt")
# grad_gxy_halide = np.transpose(grad_gxy_halide.reshape((3258+2, 2442+2)), (1, 0)) # reshape: (h, w)

# trace_heterocl = np.load("trace_heterocl.npy")