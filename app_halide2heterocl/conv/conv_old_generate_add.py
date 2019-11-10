import heterocl as hcl
hcl.init()




def _conv(_input, _filter, _bias, ):
    def _conv_0(_conv_s0_x, _conv_s0_y, _conv_s0_z, _conv_s0_n, _input, _filter, _bias, ):
        out = hcl.local(init = 0)
        out[0] = _bias[_conv_s0_z]
        return out[0]
    _conv = hcl.compute((64, 64, 32, 4, ), lambda _conv_s0_x, _conv_s0_y, _conv_s0_z, _conv_s0_n: _conv_0(_conv_s0_x, _conv_s0_y, _conv_s0_z, _conv_s0_n, _input, _filter, _bias, ), name = "_conv_0")

    def _conv_1(_conv_s1_x, _conv_s1_y, _conv_s1_z, _conv_s1_n, _input, _filter, _bias, _conv, ):
        out = hcl.local(init = _conv[_conv_s1_x, _conv_s1_y, _conv_s1_z, _conv_s1_n])
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _conv_s1_r__x = hcl.reduce_axis(0, 3)
        _conv_s1_r__y = hcl.reduce_axis(0, 3)
        _conv_s1_r__z = hcl.reduce_axis(0, 32)
        out[0] = (out[0] + _sum(
            axis = [_conv_s1_r__z, _conv_s1_r__y, _conv_s1_r__x, ],
            expr = (_filter[((_conv_s1_z * 288) + ((_conv_s1_r__z * 9) + ((_conv_s1_r__y * 3) + _conv_s1_r__x)))] * _input[((_conv_s1_n * 143648) + ((_conv_s1_r__z * 4489) + (((_conv_s1_r__y + _conv_s1_y) * 67) + (_conv_s1_r__x + _conv_s1_x))))])
        ))
        return out[0]
    _conv = hcl.compute((64, 64, 32, 4, ), lambda _conv_s1_x, _conv_s1_y, _conv_s1_z, _conv_s1_n: _conv_1(_conv_s1_x, _conv_s1_y, _conv_s1_z, _conv_s1_n, _input, _filter, _bias, _conv, ), name = "_conv_1")

    def _conv_2(_conv_s2_x, _conv_s2_y, _conv_s2_z, _conv_s2_n, _input, _filter, _bias, _conv, ):
        out = hcl.local(init = _conv[_conv_s2_x, _conv_s2_y, _conv_s2_z, _conv_s2_n])
        out[0] = hcl.select(out[0] > float(0.000000), out[0], float(0.000000))
        return out[0]
    _conv = hcl.compute((64, 64, 32, 4, ), lambda _conv_s2_x, _conv_s2_y, _conv_s2_z, _conv_s2_n: _conv_2(_conv_s2_x, _conv_s2_y, _conv_s2_z, _conv_s2_n, _input, _filter, _bias, _conv, ), name = "_conv_2")

    return _conv

_bias = hcl.placeholder((32, ), name = "_bias")
_filter = hcl.placeholder((3 * 3 * 32 * 32, ), name = "_filter")
_input = hcl.placeholder((67 * 67 * 32 * 4, ), name = "_input")
s = hcl.create_schedule([_input, _filter, _bias, ], _conv)

# change the default order
s_0 = _conv._conv_0
s[s_0].reorder(s_0.axis[3], s_0.axis[2], s_0.axis[1], s_0.axis[0])
s_1 = _conv._conv_1
s[s_1].reorder(s_1.axis[3], s_1.axis[2], s_1.axis[1], s_1.axis[0])
s_2 = _conv._conv_2
s[s_2].reorder(s_2.axis[3], s_2.axis[2], s_2.axis[1], s_2.axis[0])

f = hcl.build(s)
print(hcl.lower(s))

import numpy as np 
np_in = np.transpose(np.load("input.npy"), (3,2,1,0)).flatten()
np_filter = np.transpose(np.load("filter.npy"), (3,2,1,0)).flatten()
np_bias = np.transpose(np.load("bias.npy"), (0)).flatten()

hcl_in = hcl.asarray(np_in)
hcl_filter = hcl.asarray(np_filter)
hcl_bias = hcl.asarray(np_bias)
hcl_out = hcl.asarray(np.zeros((64, 64, 32, 4)))

f(hcl_in, hcl_filter, hcl_bias, hcl_out)

np_out = hcl_out.asnumpy()
np_out_64 = np_out[0:64, 0:64, :, :]
np.save("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl_generate.npy", np_out_64)