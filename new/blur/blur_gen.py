import heterocl as hcl
hcl.init()
final_extent_0 = 640
final_extent_1 = 480
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    blur_x = hcl.compute((final_extent_0, (final_extent_1 + 2)), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_x"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "blur_x_s0_y") as blur_x_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "blur_x_s0_x") as blur_x_s0_x:
                blur_x[blur_x_s0_x, blur_x_s0_y] = ((input[(blur_x_s0_x + 2), blur_x_s0_y] + (input[blur_x_s0_x, blur_x_s0_y] + input[(blur_x_s0_x + 1), blur_x_s0_y]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3))
    blur_y = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "blur_y", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_y"):
        with hcl.for_(final_min_1, final_extent_1, name = "blur_y_s0_y") as blur_y_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "blur_y_s0_x") as blur_y_s0_x:
                blur_y[blur_y_s0_x, blur_y_s0_y] = ((blur_x[blur_y_s0_x, (blur_y_s0_y + 2)] + (blur_x[blur_y_s0_x, blur_y_s0_y] + blur_x[blur_y_s0_x, (blur_y_s0_y + 1)]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3))
    final = hcl.compute((640, 480), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = blur_y[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((648, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
# schedule
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (640, 480, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
