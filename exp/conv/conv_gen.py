import heterocl as hcl
hcl.init()
final_extent_0 = 64
final_extent_1 = 64
final_extent_2 = 32
final_extent_3 = 4
final_min_0 = 0
final_min_1 = 0
final_min_2 = 0
final_min_3 = 0
def top(input, filter, bias, ):
    f_conv = hcl.compute((final_extent_0, final_extent_1, final_extent_2, final_extent_3), lambda x, y, z, w: 0, name = "f_conv", dtype = hcl.Float(bits = 32))
    with hcl.Stage("f_conv"):
        with hcl.for_(final_min_3, final_extent_3, name = "f_conv_s0_n") as f_conv_s0_n:
            with hcl.for_(final_min_2, final_extent_2, name = "f_conv_s0_z") as f_conv_s0_z:
                with hcl.for_(final_min_1, final_extent_1, name = "f_conv_s0_y") as f_conv_s0_y:
                    with hcl.for_(final_min_0, final_extent_0, name = "f_conv_s0_x") as f_conv_s0_x:
                        f_conv[f_conv_s0_x, f_conv_s0_y, f_conv_s0_z, f_conv_s0_n] = bias[f_conv_s0_z]
        with hcl.for_(final_min_3, final_extent_3, name = "f_conv_s1_n") as f_conv_s1_n:
            with hcl.for_(final_min_2, final_extent_2, name = "f_conv_s1_z") as f_conv_s1_z:
                with hcl.for_(final_min_1, final_extent_1, name = "f_conv_s1_y") as f_conv_s1_y:
                    with hcl.for_(final_min_0, final_extent_0, name = "f_conv_s1_x") as f_conv_s1_x:
                        with hcl.for_(0, 32, name = "f_conv_s1_r__z") as f_conv_s1_r__z:
                            with hcl.for_(0, 3, name = "f_conv_s1_r__y") as f_conv_s1_r__y:
                                with hcl.for_(0, 3, name = "f_conv_s1_r__x") as f_conv_s1_r__x:
                                    f_conv[f_conv_s1_x, f_conv_s1_y, f_conv_s1_z, f_conv_s1_n] = (f_conv[f_conv_s1_x, f_conv_s1_y, f_conv_s1_z, f_conv_s1_n] + (filter[f_conv_s1_r__x, f_conv_s1_r__y, f_conv_s1_r__z, f_conv_s1_z] * input[(f_conv_s1_r__x + f_conv_s1_x), (f_conv_s1_r__y + f_conv_s1_y), f_conv_s1_r__z, f_conv_s1_n]))
    f_relu = hcl.compute((final_extent_0, final_extent_1, final_extent_2, final_extent_3), lambda x, y, z, w: 0, name = "f_relu", dtype = hcl.Float(bits = 32))
    with hcl.Stage("f_relu"):
        with hcl.for_(final_min_3, final_extent_3, name = "f_relu_s0_n") as f_relu_s0_n:
            with hcl.for_(final_min_2, final_extent_2, name = "f_relu_s0_z") as f_relu_s0_z:
                with hcl.for_(final_min_1, final_extent_1, name = "f_relu_s0_y") as f_relu_s0_y:
                    with hcl.for_(final_min_0, final_extent_0, name = "f_relu_s0_x") as f_relu_s0_x:
                        f_relu[f_relu_s0_x, f_relu_s0_y, f_relu_s0_z, f_relu_s0_n] = hcl.select(f_conv[f_relu_s0_x, f_relu_s0_y, f_relu_s0_z, f_relu_s0_n] > hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000), f_conv[f_relu_s0_x, f_relu_s0_y, f_relu_s0_z, f_relu_s0_n], hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000))
    final = hcl.compute((64, 64, 32, 4), lambda x, y, z, w: 0, name = "final", dtype = hcl.Float(bits = 32))
    with hcl.Stage("final"):
        with hcl.for_(final_min_3, final_extent_3, name = "final_s0_n") as final_s0_n:
            with hcl.for_(final_min_2, final_extent_2, name = "final_s0_z") as final_s0_z:
                with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
                    with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                        final[final_s0_x, final_s0_y, final_s0_z, final_s0_n] = f_relu[final_s0_x, final_s0_y, final_s0_z, final_s0_n]
    return final
input = hcl.placeholder((67, 67, 32, 4, ), name = "input", dtype = hcl.Float(bits = 32))
filter = hcl.placeholder((3, 3, 32, 32, ), name = "filter", dtype = hcl.Float(bits = 32))
bias = hcl.placeholder((32, ), name = "bias", dtype = hcl.Float(bits = 32))
s = hcl.create_schedule([input, filter, bias, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.Float(bits = 32))
np_filter = np.load("filter.npy")
hcl_filter = hcl.asarray(np_filter, dtype = hcl.Float(bits = 32))
np_bias = np.load("bias.npy")
hcl_bias = hcl.asarray(np_bias, dtype = hcl.Float(bits = 32))
output_shape = (64, 64, 32, 4, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
f(hcl_input, hcl_filter, hcl_bias, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
# print(hcl.build(s, target = "soda"))
print(hcl.build(s, target = "merlinc"))
