import heterocl as hcl
hcl.init()
final_extent_0 = 1024
final_extent_1 = 1024
final_min_0 = 0
final_min_1 = 0
def top(A, B, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    final = hcl.compute((1024, 1024), lambda x, y: 0, name = "final", dtype = (bits = 0))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                prod = hcl.compute((8, 8), lambda x, y: 0, name = "prod", dtype = hcl.Int(bits = 16))
                with hcl.Stage("prod"):
                    prod[final_s0_x, final_s0_y] = hcl.cast(dtype = hcl.Int(bits = 16), expr = 0)
                    with hcl.for_(0, 8, name = "prod_s1_y_yi") as prod_s1_y_yi:
                        with hcl.for_(0, 8, name = "prod_s1_x_xi") as prod_s1_x_xi:
                            with hcl.for_(0, 1024, name = "prod_s1_r__x") as prod_s1_r__x:
                                prod[(final_s0_x + prod_s1_x_xi), (final_s0_y + prod_s1_y_yi)] = (prod[(final_s0_x + prod_s1_x_xi), (final_s0_y + prod_s1_y_yi)] + (A[(final_s0_x + prod_s1_x_xi), prod_s1_r__x] * B[prod_s1_r__x, (final_s0_y + prod_s1_y_yi)]))
                final[final_s0_x, final_s0_y] = prod[final_s0_x, final_s0_y]
    return final
A = hcl.placeholder((1024, 1024, ), name = "A", dtype = hcl.Int(bits = 16))
B = hcl.placeholder((1024, 1024, ), name = "B", dtype = hcl.Int(bits = 16))
s = hcl.create_schedule([A, B, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_A = np.load("A.npy")
hcl_A = hcl.asarray(np_A, dtype = hcl.Int(bits = 16))
np_B = np.load("B.npy")
hcl_B = hcl.asarray(np_B, dtype = hcl.Int(bits = 16))
output_shape = (1024, 1024, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Int(bits = 16))
f(hcl_A, hcl_B, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
