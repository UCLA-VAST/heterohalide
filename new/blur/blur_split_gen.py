import heterocl as hcl
hcl.init()
final_extent_0 = 640
final_extent_1 = 480
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    blur_x = hcl.compute((hcl.select(final_extent_0 > 200, final_extent_0, 200), (hcl.select(final_extent_1 > 100, final_extent_1, 100) + 2)), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_x"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "blur_x_s0_y") as blur_x_s0_y:
            with hcl.for_(0, ((final_extent_0 + 199)//200), name = "blur_x_s0_x_x_outer") as blur_x_s0_x_x_outer:
                blur_x_s0_x_x_inner_base_s = hcl.select((blur_x_s0_x_x_outer * 200) < (final_extent_0 + -200), (blur_x_s0_x_x_outer * 200), (final_extent_0 + -200))
                with hcl.for_(0, 200, name = "blur_x_s0_x_x_inner") as blur_x_s0_x_x_inner:
                    blur_x[((blur_x_s0_x_x_inner_base_s + final_min_0) + blur_x_s0_x_x_inner), blur_x_s0_y] = ((input[(((blur_x_s0_x_x_inner_base_s + final_min_0) + blur_x_s0_x_x_inner) + 2), blur_x_s0_y] + (input[((blur_x_s0_x_x_inner_base_s + final_min_0) + blur_x_s0_x_x_inner), blur_x_s0_y] + input[(((blur_x_s0_x_x_inner_base_s + final_min_0) + blur_x_s0_x_x_inner) + 1), blur_x_s0_y]))//hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3))
    blur_y = hcl.compute((final_extent_0, hcl.select(final_extent_1 > 100, final_extent_1, 100)), lambda x, y: 0, name = "blur_y", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_y"):
        with hcl.for_(0, ((final_extent_1 + 99)//100), name = "blur_y_s0_y_y_outer") as blur_y_s0_y_y_outer:
            blur_y_s0_y_y_inner_base_s = hcl.select((blur_y_s0_y_y_outer * 100) < (final_extent_1 + -100), (blur_y_s0_y_y_outer * 100), (final_extent_1 + -100))
            with hcl.for_(0, 100, name = "blur_y_s0_y_y_inner") as blur_y_s0_y_y_inner:
                with hcl.for_(final_min_0, final_extent_0, name = "blur_y_s0_x") as blur_y_s0_x:
                    blur_y[blur_y_s0_x, ((blur_y_s0_y_y_inner_base_s + final_min_1) + blur_y_s0_y_y_inner)] = ((blur_x[blur_y_s0_x, (((blur_y_s0_y_y_inner_base_s + final_min_1) + blur_y_s0_y_y_inner) + 2)] + (blur_x[blur_y_s0_x, ((blur_y_s0_y_y_inner_base_s + final_min_1) + blur_y_s0_y_y_inner)] + blur_x[blur_y_s0_x, (((blur_y_s0_y_y_inner_base_s + final_min_1) + blur_y_s0_y_y_inner) + 1)]))//hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3))
    final = hcl.compute((640, 480), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = blur_y[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((648, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
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
