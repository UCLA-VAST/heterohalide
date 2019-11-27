# # hcl.select() and definite number comparison
# import heterocl as hcl 
# hcl.init()
# extent = 10
# def top(input, ):
#     blur_x = hcl.compute((640, (480 + 2)), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16))
#     with hcl.Stage("s1"):
#         blur_x[hcl.select(10 > 1, 1, 0), 5] = 2
#     return blur_x

# input = hcl.placeholder((642, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
# s = hcl.create_schedule([input, ], top)
# print(hcl.lower(s))
# f = hcl.build(s)



# split schedule -> division operation in hcl.for_()
import heterocl as hcl 
hcl.init()
extent = 10
def top(input, ):
    blur_x = hcl.compute((640, (480 + 2)), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_x"):
        with hcl.for_(0, 640 // 1, name = "x") as x:
            with hcl.for_(0, (480 + 2), name = "y") as y:
                blur_x[x, y] = ((input[(x + 2), y] + input[x, y] + input[(x + 1), y])/3) + hcl.select(extent > 1, extent, 1)
                
    # blur_x[hcl.select(10 > 1, 10, 1), 0] = 2 
    with hcl.Stage("s1"):
        blur_x[hcl.select(10 > 1, 1, 0), 5] = 2
    return blur_x

input = hcl.placeholder((642, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
print(hcl.lower(s))
f = hcl.build(s)