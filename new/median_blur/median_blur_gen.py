import heterocl as hcl
hcl.init()
final_extent_0 = 6418
final_extent_1 = 4818
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    mean_local = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "mean_local", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("mean_local"):
        with hcl.for_(final_min_1, final_extent_1, name = "mean_local_s0_y") as mean_local_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "mean_local_s0_x") as mean_local_s0_x:
                mean_local[mean_local_s0_x, mean_local_s0_y] = hcl.cast(dtype = hcl.UInt(bits = 16), expr = 0)
        with hcl.for_(final_min_1, final_extent_1, name = "mean_local_s1_y") as mean_local_s1_y:
            with hcl.for_(final_min_0, final_extent_0, name = "mean_local_s1_x") as mean_local_s1_x:
                with hcl.for_(0, 3, name = "mean_local_s1_box__y") as mean_local_s1_box__y:
                    with hcl.for_(0, 3, name = "mean_local_s1_box__x") as mean_local_s1_box__x:
                        mean_local[mean_local_s1_x, mean_local_s1_y] = (mean_local[mean_local_s1_x, mean_local_s1_y] + (input[(mean_local_s1_box__x + mean_local_s1_x), (mean_local_s1_box__y + mean_local_s1_y)]//hcl.cast(dtype = hcl.UInt(bits = 16), expr = 9)))
    final = hcl.compute((6418, 4818), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = mean_local[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((6420, 4820, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (6418, 4818, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
print(hcl.build(s, target = "merlinc"))
