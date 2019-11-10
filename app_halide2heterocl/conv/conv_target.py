import heterocl as hcl
hcl.init()
_relu_extent_0 = 64
_relu_extent_1 = 64
_relu_extent_2 = 32
_relu_extent_3 = 4
def _all(_input, _filter, _bias, ):
    def _f_conv_0(_f_conv_s0_x, _f_conv_s0_y, _f_conv_s0_z, _f_conv_s0_n, _input, _filter, _bias, ):
        out = hcl.local(0)
        out[0] = _bias[_f_conv_s0_z]
        return out[0]
    _f_conv_0 = hcl.compute((_relu_extent_0, _relu_extent_1, _relu_extent_2, _relu_extent_3, ), lambda _f_conv_s0_x, _f_conv_s0_y, _f_conv_s0_z, _f_conv_s0_n, : _f_conv_0(_f_conv_s0_x, _f_conv_s0_y, _f_conv_s0_z, _f_conv_s0_n, _input, _filter, _bias, ), name = "_f_conv_0")

    def _f_conv_1(_f_conv_s1_x, _f_conv_s1_y, _f_conv_s1_z, _f_conv_s1_n, _input, _filter, _bias, _f_conv, ): # for loop
        out = hcl.local(0) # provide
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _f_conv_s1_r__x = hcl.reduce_axis(0, 3)
        _f_conv_s1_r__y = hcl.reduce_axis(0, 3)
        _f_conv_s1_r__z = hcl.reduce_axis(0, 32)
        out[0] = (_f_conv[_f_conv_s1_x, _f_conv_s1_y, _f_conv_s1_z, _f_conv_s1_n] + _sum( # provide + op: where to begin reducer_name is tricky here. Just set it to the first binop operaion's second Expr
            axis = [_f_conv_s1_r__z, _f_conv_s1_r__y, _f_conv_s1_r__x, ],
            expr = (_filter[_f_conv_s1_r__x, _f_conv_s1_r__y, _f_conv_s1_r__z, _f_conv_s1_z] * _input[_f_conv_s1_r__x + _f_conv_s1_x, _f_conv_s1_r__y + _f_conv_s1_y, _f_conv_s1_r__z, _f_conv_s1_n])
        ))
        return out[0]
    _f_conv_1 = hcl.compute((_relu_extent_0, _relu_extent_1, _relu_extent_2, _relu_extent_3, ), lambda _f_conv_s1_x, _f_conv_s1_y, _f_conv_s1_z, _f_conv_s1_n, : _f_conv_1(_f_conv_s1_x, _f_conv_s1_y, _f_conv_s1_z, _f_conv_s1_n, _input, _filter, _bias, _f_conv_0), name = "_f_conv_1") # need _f_conv_1 instead of _f_conv

    def _relu_0(_relu_s0_x, _relu_s0_y, _relu_s0_z, _relu_s0_n, _f_conv):
        out = hcl.local(0)
        out[0] = hcl.select(_f_conv[_relu_s0_x, _relu_s0_y, _relu_s0_z, _relu_s0_n] > float(0.000000), _f_conv[_relu_s0_x, _relu_s0_y, _relu_s0_z, _relu_s0_n], float(0.000000))
        return out[0]
    _relu_0 = hcl.compute((_relu_extent_0, _relu_extent_1, _relu_extent_2, _relu_extent_3, ), lambda _relu_s0_x, _relu_s0_y, _relu_s0_z, _relu_s0_n, : _relu_0(_relu_s0_x, _relu_s0_y, _relu_s0_z, _relu_s0_n, _f_conv_1), name = "_relu_0") # need _f_conv_1 instead of _f_conv

    return _relu_0
_bias = hcl.placeholder((32, ), name = "_bias")
_filter = hcl.placeholder((3 , 3 , 32 , 32, ), name = "_filter")
_input = hcl.placeholder((67 , 67 , 32 , 4, ), name = "_input")
s = hcl.create_schedule([_input, _filter, _bias, ], _all)
f = hcl.build(s)
print(hcl.lower(s))

import numpy as np 
np_in = np.load("input.npy")
np_filter = np.load("filter.npy")
np_bias = np.load("bias.npy")

hcl_in = hcl.asarray(np_in)
hcl_filter = hcl.asarray(np_filter)
hcl_bias = hcl.asarray(np_bias)
hcl_out = hcl.asarray(np.zeros((64, 64, 32, 4)))

f(hcl_in, hcl_filter, hcl_bias, hcl_out)

np_out = hcl_out.asnumpy()

np.save("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl_generate_root_target.npy", np_out)