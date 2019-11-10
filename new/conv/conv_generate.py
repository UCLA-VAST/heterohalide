import heterocl as hcl
hcl.init()
output_extent_0 = 64
output_extent_1 = 64
output_extent_2 = 32
output_extent_3 = 4
output_min_0 = 0
output_min_1 = 0
output_min_2 = 0
output_min_3 = 0
def _all(input, filter, bias, ):
    def f_conv_0(f_conv_s0_n, f_conv_s0_z, f_conv_s0_y, f_conv_s0_x, input, filter, bias, ):
        return bias[f_conv_s0_z]
    f_conv_0 = hcl.compute((output_extent_3, output_extent_2, output_extent_1, output_extent_0, ), lambda f_conv_s0_n, f_conv_s0_z, f_conv_s0_y, f_conv_s0_x, : f_conv_0(f_conv_s0_n, f_conv_s0_z, f_conv_s0_y, f_conv_s0_x, input, filter, bias, ), name = "f_conv_0", dtype = hcl.Float(bits = 32))

    def f_conv_1(f_conv_s1_n, f_conv_s1_z, f_conv_s1_y, f_conv_s1_x, input, filter, bias, f_conv, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        f_conv_s1_r__x = hcl.reduce_axis(0, 3)
        f_conv_s1_r__y = hcl.reduce_axis(0, 3)
        f_conv_s1_r__z = hcl.reduce_axis(0, 32)
        return (f_conv[f_conv_s1_n, f_conv_s1_z, f_conv_s1_y, f_conv_s1_x] + _sum(
            axis = [f_conv_s1_r__z, f_conv_s1_r__y, f_conv_s1_r__x, ],
            expr = (filter[f_conv_s1_z, f_conv_s1_r__z, f_conv_s1_r__y, f_conv_s1_r__x] * input[f_conv_s1_n, f_conv_s1_r__z, (f_conv_s1_r__y + f_conv_s1_y), (f_conv_s1_r__x + f_conv_s1_x)])
        ))
    f_conv_1 = hcl.compute((output_extent_3, output_extent_2, output_extent_1, output_extent_0, ), lambda f_conv_s1_n, f_conv_s1_z, f_conv_s1_y, f_conv_s1_x, : f_conv_1(f_conv_s1_n, f_conv_s1_z, f_conv_s1_y, f_conv_s1_x, input, filter, bias, f_conv_0, ), name = "f_conv_1", dtype = hcl.Float(bits = 32))

    def f_relu_0(f_relu_s0_n, f_relu_s0_z, f_relu_s0_y, f_relu_s0_x, input, filter, bias, f_conv, ):
        return hcl.select(f_conv[f_relu_s0_n, f_relu_s0_z, f_relu_s0_y, f_relu_s0_x] > hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000), f_conv[f_relu_s0_n, f_relu_s0_z, f_relu_s0_y, f_relu_s0_x], hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000))
    f_relu_0 = hcl.compute((output_extent_3, output_extent_2, output_extent_1, output_extent_0, ), lambda f_relu_s0_n, f_relu_s0_z, f_relu_s0_y, f_relu_s0_x, : f_relu_0(f_relu_s0_n, f_relu_s0_z, f_relu_s0_y, f_relu_s0_x, input, filter, bias, f_conv_1, ), name = "f_relu_0", dtype = hcl.Float(bits = 32))

    def output_0(output_s0_n, output_s0_z, output_s0_y, output_s0_x, input, filter, bias, f_relu, ):
        return f_relu[output_s0_n, output_s0_z, output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_3, output_extent_2, output_extent_1, output_extent_0, ), lambda output_s0_n, output_s0_z, output_s0_y, output_s0_x, : output_0(output_s0_n, output_s0_z, output_s0_y, output_s0_x, input, filter, bias, f_relu_0, ), name = "output_0", dtype = hcl.Float(bits = 32))

    return output_0
input = hcl.placeholder((4, 32, 67, 67, ), name = "input", dtype = hcl.Float(bits = 32))
filter = hcl.placeholder((32, 32, 3, 3, ), name = "filter", dtype = hcl.Float(bits = 32))
bias = hcl.placeholder((32, ), name = "bias", dtype = hcl.Float(bits = 32))
s = hcl.create_schedule([input, filter, bias, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (3, 2, 1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.Float(bits = 32))
np_filter = np.transpose(np.load("filter.npy"), (3, 2, 1, 0))
hcl_filter = hcl.asarray(np_filter, dtype = hcl.Float(bits = 32))
np_bias = np.transpose(np.load("bias.npy"), (0))
hcl_bias = hcl.asarray(np_bias, dtype = hcl.Float(bits = 32))
output_shape = (4, 32, 64, 64)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
f(hcl_input, hcl_filter, hcl_bias, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (3, 2, 1, 0))
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
