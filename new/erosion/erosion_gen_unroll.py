import heterocl as hcl
hcl.init()
final_extent_0 = 640
final_extent_1 = 480
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    final_extent_0_required_s = (hcl.select((((final_extent_0 + -1)//4) * 4) < (final_extent_0 + -4), (((final_extent_0 + -1)//4) * 4), (final_extent_0 + -4)) - hcl.select(final_extent_0 < 4, final_extent_0, 4))
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    min_local = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "min_local", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("min_local"):
        with hcl.for_(final_min_1, final_extent_1, name = "min_local_s0_y") as min_local_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "min_local_s0_x") as min_local_s0_x:
                minimum = hcl.compute((1, 1), lambda x, y: 0, name = "minimum", dtype = hcl.UInt(bits = 16))
                with hcl.Stage("minimum"):
                    minimum[min_local_s0_x, min_local_s0_y] = hcl.cast(dtype = hcl.UInt(bits = 16), expr = 65535)
                    with hcl.for_(0, 3, name = "minimum_s1_box__y") as minimum_s1_box__y:
                        with hcl.for_(0, 3, name = "minimum_s1_box__x") as minimum_s1_box__x:
                            minimum[min_local_s0_x, min_local_s0_y] = hcl.select(minimum[min_local_s0_x, min_local_s0_y] < input[(min_local_s0_x + minimum_s1_box__x), (min_local_s0_y + minimum_s1_box__y)], minimum[min_local_s0_x, min_local_s0_y], input[(min_local_s0_x + minimum_s1_box__x), (min_local_s0_y + minimum_s1_box__y)])
                min_local[min_local_s0_x, min_local_s0_y] = minimum[min_local_s0_x, min_local_s0_y]
    final = hcl.compute((640, 480), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(0, ((final_extent_0 + 3)//4), name = "final_s0_x_x") as final_s0_x_x:
                final_s0_x_v10_base_s = hcl.select((final_s0_x_x * 4) < (final_extent_0 + -4), (final_s0_x_x * 4), (final_extent_0 + -4))
                with hcl.for_(0, 4, name = "final_s0_x_v10") as final_s0_x_v10:
                    final[((final_min_0 + final_s0_x_v10_base_s) + final_s0_x_v10), final_s0_y] = min_local[((final_min_0 + final_s0_x_v10_base_s) + final_s0_x_v10), final_s0_y]
    return final
input = hcl.placeholder((642, 482, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
s_final = top.final
s[s_final].unroll(s_final.axis[2], 0)
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
