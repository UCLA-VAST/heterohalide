import heterocl as hcl 
hcl.init()
blur_y_extent_0 = 6472
blur_y_extent_1 = 4818
blur_y_min_0 = 0
blur_y_min_1 = 0
input = hcl.placeholder((6480, 4820), name = "input", dtype = hcl.UInt(bits = 16))
def top(input):
    blur_x = hcl.compute((blur_y_extent_0, blur_y_extent_1 + 2), lambda x, y: 0, name = "blur_x") # realize: it's like allocate
    blur_y = hcl.compute((blur_y_extent_0, blur_y_extent_1), lambda x, y: 0, name = "blur_y") # no corresponding realize node in Halide IR for the output, but we can create a compute function for all the output?
    with hcl.Stage("stage_blur_x"):  # produce
        with hcl.for_(blur_y_min_1, blur_y_extent_1 + 2, name = "blur_x_s0_y") as blur_x_s0_y:
            with hcl.for_(blur_y_min_0, blur_y_extent_0, name = "blur_x_s0_x") as blur_x_s0_x:
                blur_x[blur_x_s0_x, blur_x_s0_y] = ((input[(blur_x_s0_x + 2), blur_x_s0_y] + (input[blur_x_s0_x, blur_x_s0_y] + input[(blur_x_s0_x + 1), blur_x_s0_y])) / 3)
    with hcl.Stage("stage_blur_y"): # for
        with hcl.for_(blur_y_min_1, blur_y_extent_1, name = "blur_y_s0_y") as blur_y_s0_y:
            with hcl.for_(blur_y_min_0, blur_y_extent_0, name = "blur_y_s0_x") as blur_y_s0_x:
                blur_y[blur_y_s0_x, blur_y_s0_y] = ((blur_x[blur_y_s0_x, (blur_y_s0_y + 2)] + (blur_x[blur_y_s0_x, blur_y_s0_y] + blur_x[blur_y_s0_x, (blur_y_s0_y + 1)])) / 3)  
    return blur_y # the last Load? 

                # see hcl.update, because for multi-input and multi-output, need to use hcl.update to support it
s = hcl.create_schedule([input], top)

s_blur_x = top.blur_x
s[s_blur_x].unroll(s_blur_x.axis[0], 2) # it works

stage_blur_x = top.stage_blur_x
s[stage_blur_x].unroll(stage_blur_x.blur_x_s0_y, 2) # it works

print(hcl.lower(s))
f = hcl.build(s)
print(hcl.build(s, target = "soda"))


# Stage is similar to a compute function