import heterocl as hcl
hcl.init(hcl.Float(bits = 32))
_f_relu_extent_0 = 64
_f_relu_extent_1 = 64
_f_relu_extent_2 = 32
_f_relu_extent_3 = 4
_f_relu_min_0 = 0
_f_relu_min_1 = 0
_f_relu_min_2 = 0
_f_relu_min_3 = 0
def _all(_input, _filter, _bias, ):
    def _f_conv_0(_f_conv_s0_n, _f_conv_s0_z, _f_conv_s0_y, _f_conv_s0_x, _input, _filter, _bias, ):
        return _bias[_f_conv_s0_z]
    _f_conv_0 = hcl.compute((_f_relu_extent_3, _f_relu_extent_2, _f_relu_extent_1, _f_relu_extent_0, ), lambda _f_conv_s0_n, _f_conv_s0_z, _f_conv_s0_y, _f_conv_s0_x, : _f_conv_0(_f_conv_s0_n, _f_conv_s0_z, _f_conv_s0_y, _f_conv_s0_x, _input, _filter, _bias, ), name = "_f_conv_0")

    def _f_conv_1(_f_conv_s1_n, _f_conv_s1_z, _f_conv_s1_y, _f_conv_s1_x, _input, _filter, _bias, _f_conv, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _f_conv_s1_r__x = hcl.reduce_axis(0, 3)
        _f_conv_s1_r__y = hcl.reduce_axis(0, 3)
        _f_conv_s1_r__z = hcl.reduce_axis(0, 32)
        return (_f_conv[_f_conv_s1_n, _f_conv_s1_z, _f_conv_s1_y, _f_conv_s1_x] + _sum(
            axis = [_f_conv_s1_r__z, _f_conv_s1_r__y, _f_conv_s1_r__x, ],
            expr = (_filter[_f_conv_s1_z, _f_conv_s1_r__z, _f_conv_s1_r__y, _f_conv_s1_r__x] * _input[_f_conv_s1_n, _f_conv_s1_r__z, (_f_conv_s1_r__y + _f_conv_s1_y), (_f_conv_s1_r__x + _f_conv_s1_x)])
        ))
    _f_conv_1 = hcl.compute((_f_relu_extent_3, _f_relu_extent_2, _f_relu_extent_1, _f_relu_extent_0, ), lambda _f_conv_s1_n, _f_conv_s1_z, _f_conv_s1_y, _f_conv_s1_x, : _f_conv_1(_f_conv_s1_n, _f_conv_s1_z, _f_conv_s1_y, _f_conv_s1_x, _input, _filter, _bias, _f_conv_0, ), name = "_f_conv_1")

    def _f_relu_0(_f_relu_s0_n, _f_relu_s0_z, _f_relu_s0_y, _f_relu_s0_x, _input, _filter, _bias, _f_conv, ):
        return hcl.select(_f_conv[_f_relu_s0_n, _f_relu_s0_z, _f_relu_s0_y, _f_relu_s0_x] > float(0.000000), _f_conv[_f_relu_s0_n, _f_relu_s0_z, _f_relu_s0_y, _f_relu_s0_x], float(0.000000))
    _f_relu_0 = hcl.compute((_f_relu_extent_3, _f_relu_extent_2, _f_relu_extent_1, _f_relu_extent_0, ), lambda _f_relu_s0_n, _f_relu_s0_z, _f_relu_s0_y, _f_relu_s0_x, : _f_relu_0(_f_relu_s0_n, _f_relu_s0_z, _f_relu_s0_y, _f_relu_s0_x, _input, _filter, _bias, _f_conv_1, ), name = "_f_relu_0")

    return _f_relu_0
_input = hcl.placeholder((4, 32, 67, 67, ), name = "_input")
_filter = hcl.placeholder((32, 32, 3, 3, ), name = "_filter")
_bias = hcl.placeholder((32, ), name = "_bias")
s = hcl.create_schedule([_input, _filter, _bias, ], _all)
s_f_conv_0 = _all._f_conv_0
s[s_f_conv_0].unroll(s_f_conv_0.axis[3], 2)
s_f_relu_0 = _all._f_relu_0
s[s_f_relu_0].unroll(s_f_relu_0.axis[3], 2)
# s[s_f_conv_0].systolic() # Stage object has no attribute 'systolic'
# s[s_f_relu_0].systolic()
f = hcl.build(s)
print(hcl.lower(s))
# import numpy as np
# np_input = np.transpose(np.load("_input.npy"), (3, 2, 1, 0))
# hcl_input = hcl.asarray(np_input)
# np_filter = np.transpose(np.load("_filter.npy"), (3, 2, 1, 0))
# hcl_filter = hcl.asarray(np_filter)
# np_bias = np.transpose(np.load("_bias.npy"), (0))
# hcl_bias = hcl.asarray(np_bias)
# output_shape = (4, 32, 64, 64)
# hcl_out = hcl.asarray(np.zeros(output_shape))
# f(hcl_input, hcl_filter, hcl_bias, hcl_out)
# np_out = hcl_out.asnumpy()
# np_out = np.transpose(np_out, (3, 2, 1, 0))
print(hcl.build(s, target="vhls"))
print(hcl.build(s, target="ihls"))
print(hcl.build(s, target="merlinc"))