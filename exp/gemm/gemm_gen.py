import heterocl as hcl
hcl.init()
final_extent_0 = 1024
final_extent_1 = 1024
final_min_0 = 0
final_min_1 = 0
def top(A, B, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    prod = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "prod", dtype = hcl.Float(bits = 32))
    with hcl.Stage("prod"):
        with hcl.for_(final_min_1, final_extent_1, name = "prod_s0_y") as prod_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "prod_s0_x") as prod_s0_x:
                prod[prod_s0_x, prod_s0_y] = hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000)
        with hcl.for_(final_min_1, final_extent_1, name = "prod_s1_y") as prod_s1_y:
            with hcl.for_(final_min_0, final_extent_0, name = "prod_s1_x") as prod_s1_x:
                with hcl.for_(0, 1024, name = "prod_s1_r__x") as prod_s1_r__x:
                    prod[prod_s1_x, prod_s1_y] = (prod[prod_s1_x, prod_s1_y] + (A[prod_s1_x, prod_s1_r__x] * B[prod_s1_r__x, prod_s1_y]))
    final = hcl.compute((1024, 1024), lambda x, y: 0, name = "final", dtype = hcl.Float(bits = 32))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = prod[final_s0_x, final_s0_y]
    return final
A = hcl.placeholder((1024, 1024, ), name = "A", dtype = hcl.Float(bits = 32))
B = hcl.placeholder((1024, 1024, ), name = "B", dtype = hcl.Float(bits = 32))
s = hcl.create_schedule([A, B, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
# np_A = np.load("A.npy")
# hcl_A = hcl.asarray(np_A, dtype = hcl.Float(bits = 32))
# np_B = np.load("B.npy")
# hcl_B = hcl.asarray(np_B, dtype = hcl.Float(bits = 32))
# output_shape = (1024, 1024, )
# hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
# f(hcl_A, hcl_B, hcl_out)
# np_out = hcl_out.asnumpy()
# np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "merlinc"))
