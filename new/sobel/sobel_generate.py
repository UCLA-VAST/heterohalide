import heterocl as hcl
hcl.init()
final_extent_0 = 638
final_extent_1 = 478
final_min_0 = 0
final_min_1 = 0
def top(input, filter_x, filter_y, ):
    gx = hcl.compute(((final_extent_0 + 2), (final_extent_1 + 2)), lambda x, y: 0, name = "gx", dtype = hcl.Int(bits = 16))
    with hcl.Stage("gx"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "gx_s0_y") as gx_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "gx_s0_x") as gx_s0_x:
                gx[gx_s0_x, gx_s0_y] = hcl.cast(dtype = hcl.Int(bits = 16), expr = 0)
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "gx_s1_y") as gx_s1_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "gx_s1_x") as gx_s1_x:
                with hcl.for_(0, 3, name = "gx_s1_r__y") as gx_s1_r__y:
                    with hcl.for_(0, 3, name = "gx_s1_r__x") as gx_s1_r__x:
                        gx[gx_s1_x, gx_s1_y] = (gx[gx_s1_x, gx_s1_y] + (filter_x[gx_s1_r__x, gx_s1_r__y] * input[(gx_s1_r__x + gx_s1_x), (gx_s1_r__y + gx_s1_y)]))
    gy = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "gy", dtype = hcl.Int(bits = 16))
    with hcl.Stage("gy"):
        with hcl.for_(final_min_1, final_extent_1, name = "gy_s0_y") as gy_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "gy_s0_x") as gy_s0_x:
                gy[gy_s0_x, gy_s0_y] = hcl.cast(dtype = hcl.Int(bits = 16), expr = 0)
        with hcl.for_(final_min_1, final_extent_1, name = "gy_s1_y") as gy_s1_y:
            with hcl.for_(final_min_0, final_extent_0, name = "gy_s1_x") as gy_s1_x:
                with hcl.for_(0, 3, name = "gy_s1_r__y") as gy_s1_r__y:
                    with hcl.for_(0, 3, name = "gy_s1_r__x") as gy_s1_r__x:
                        gy[gy_s1_x, gy_s1_y] = (gy[gy_s1_x, gy_s1_y] + (filter_y[gy_s1_r__x, gy_s1_r__y] * gx[(gy_s1_r__x + gy_s1_x), (gy_s1_r__y + gy_s1_y)]))
    final = hcl.compute((638, 478), lambda x, y: 0, name = "final", dtype = hcl.Int(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = gy[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((642, 482, ), name = "input", dtype = hcl.Int(bits = 16))
filter_x = hcl.placeholder((3, 3, ), name = "filter_x", dtype = hcl.Int(bits = 16))
filter_y = hcl.placeholder((3, 3, ), name = "filter_y", dtype = hcl.Int(bits = 16))
s = hcl.create_schedule([input, filter_x, filter_y, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.Int(bits = 16))
np_filter_x = np.load("filter_x.npy")
hcl_filter_x = hcl.asarray(np_filter_x, dtype = hcl.Int(bits = 16))
np_filter_y = np.load("filter_y.npy")
hcl_filter_y = hcl.asarray(np_filter_y, dtype = hcl.Int(bits = 16))
output_shape = (638, 478, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Int(bits = 16))
f(hcl_input, hcl_filter_x, hcl_filter_y, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
