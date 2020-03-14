import heterocl as hcl
hcl.init()
final_extent_0 = 640
final_extent_1 = 480
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    max_local = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "max_local", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("max_local"):
        with hcl.for_(final_min_1, final_extent_1, name = "max_local_s0_y") as max_local_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "max_local_s0_x") as max_local_s0_x:
                maximum = hcl.compute((1, 1), lambda x, y: 0, name = "maximum", dtype = hcl.UInt(bits = 16))
                with hcl.Stage("maximum"):
                    maximum[max_local_s0_x, max_local_s0_y] = hcl.cast(dtype = hcl.UInt(bits = 16), expr = 0)
                    with hcl.for_(0, 3, name = "maximum_s1_box__y") as maximum_s1_box__y:
                        with hcl.for_(0, 3, name = "maximum_s1_box__x") as maximum_s1_box__x:
                            maximum[max_local_s0_x, max_local_s0_y] = hcl.select(maximum[max_local_s0_x, max_local_s0_y] > input[(max_local_s0_x + maximum_s1_box__x), (max_local_s0_y + maximum_s1_box__y)], maximum[max_local_s0_x, max_local_s0_y], input[(max_local_s0_x + maximum_s1_box__x), (max_local_s0_y + maximum_s1_box__y)])
                max_local[max_local_s0_x, max_local_s0_y] = maximum[max_local_s0_x, max_local_s0_y]
    final = hcl.compute((640, 480), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = max_local[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((642, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
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
print(hcl.build(s, target = "merlinc"))
