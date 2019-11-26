import heterocl as hcl 
hcl.init()
def top(input, ):
    blur_x = hcl.compute((640, (480 + 2)), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_x"):
        with hcl.for_(0, 640, name = "x") as x:
            with hcl.for_(0, (480 + 2), name = "y") as y:
                blur_x[x, y] = ((input[(x + 2), y] + input[x, y] + input[(x + 1), y])/3) + hcl.select(10 > 1, 10, 1)
                
                
                # blur_x[blur_x_s0_x % 2 + blur_x_s0_x / 2, hcl.select(blur_x_s0_y > 0, blur_x_s0_y, 0)] = ((input[(blur_x_s0_x + 2), blur_x_s0_y] + (input[blur_x_s0_x, blur_x_s0_y] + input[(blur_x_s0_x + 1), blur_x_s0_y]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3)) + hcl.select(blur_x_s0_y > 1, 0, 1)
                # it's ok to have "Select" and "%" expression to represent the indexs of the tensor
    return blur_x

input = hcl.placeholder((648, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
# print(hcl.lower(s))
f = hcl.build(s)
