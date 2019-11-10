import heterocl as hcl 
hcl.init()
blur_y_extent_0 = 56 # w
blur_y_extent_1 = 46 # h
blur_y_min_0 = 0
blur_y_min_1 = 0

# Problem: in HeteroCL, storage and visiting pattern is associated with the axis order. \
# E.g. blur_x[y, x]: in HeteroCL IR, it will be blur_x[x + y * x_extent]; 
# blur_x[x, y]: in HeteroCL IR, it will be blur_x[y + x * y_extent]
# This is different with Halide IR(?).
# If storage_flatten is open, and we can see in Halide IR: blur_x[x, y] : blur_x[x + y * x_extent]

# So to solve question: 1. modify the input channel？   2. modify the axis order: 像之前一样 switch x, y order  3. 直接用storage_flatten的形式变换

# Recall: why we need to use multi-dimensional storage and load?????

def top(input):
    blur_x = hcl.compute((blur_y_extent_0, blur_y_extent_1 + 2), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16)) # realize: it's like allocate
    with hcl.Stage("stage_blur_x"):  # produce
        with hcl.for_(blur_y_min_1, blur_y_extent_1 + 2, name = "blur_x_s0_y") as blur_x_s0_y:
            with hcl.for_(blur_y_min_0, blur_y_extent_0, name = "blur_x_s0_x") as blur_x_s0_x:
                blur_x[blur_x_s0_x, blur_x_s0_y] = ((input[(blur_x_s0_x + 2), blur_x_s0_y] + (input[blur_x_s0_x, blur_x_s0_y] + input[(blur_x_s0_x + 1), blur_x_s0_y])) / 3) # Provide, Call, and other regualr calculation nodes
    blur_y = hcl.compute((blur_y_extent_0, blur_y_extent_1), lambda x, y: 0, name = "blur_y", dtype = hcl.UInt(bits = 16)) # no corresponding realize node in Halide IR for the output, but we can create a compute function for all the output?
    with hcl.Stage("stage_blur_y"): # for
        with hcl.for_(blur_y_min_1, blur_y_extent_1, name = "blur_y_s0_y") as blur_y_s0_y:
            with hcl.for_(blur_y_min_0, blur_y_extent_0, name = "blur_y_s0_x") as blur_y_s0_x:
                blur_y[blur_y_s0_x, blur_y_s0_y] = ((blur_x[blur_y_s0_x, (blur_y_s0_y + 2)] + (blur_x[blur_y_s0_x, blur_y_s0_y] + blur_x[blur_y_s0_x, (blur_y_s0_y + 1)])) / 3)  
    return blur_y # the last Load? 

input = hcl.placeholder((64, 48, ), name = "input", dtype = hcl.UInt(bits = 16))
                # see hcl.update, because for multi-input and multi-output, need to use hcl.update to support it
s = hcl.create_schedule([input], top)

# s_blur_x = top.blur_x
# s[s_blur_x].unroll(s_blur_x.axis[0], 2) # it works

# stage_blur_x = top.stage_blur_x
# s[stage_blur_x].unroll(stage_blur_x.blur_x_s0_y, 2) # it works

print(hcl.lower(s))
f = hcl.build(s)



# Stage is similar to a compute function

import numpy as np
# np_input = np.transpose(np.load("input.npy"), (1, 0))

np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (56, 46)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
# np_out = np.transpose(np_out, (1, 0))
np.save("output_target.npy", np_out)

# print(hcl.build(s, target = "soda"))