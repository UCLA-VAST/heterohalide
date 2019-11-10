import heterocl as hcl 
hcl.init()

_conv_extent_0 = 64
_conv_extent_1 = 64
_conv_extent_2 = 32
_conv_extent_3 = 4
conv_min_3 = 0
conv_min_2 = 0
conv_min_1 = 0
conv_min_0 = 0
conv_stride_3 = 32*64*64
conv_stride_2 = 64*64
conv_stride_1 = 64
conv_stride_0 = 1


def _conv(_input, _filter, _bias, ):#produce
    def _conv_0(_conv_s0_x, _conv_s0_y, _conv_s0_z, _conv_s0_n, _input, _filter, _bias, ): #for
        out = hcl.local(0) #store...
        out[0] = _bias[_conv_s0_z]
        return out[0]
    _conv = hcl.compute((64, 64, 32, 4, ), lambda _conv_s0_x, _conv_s0_y, _conv_s0_z, _conv_s0_n: _conv_0(_conv_s0_x, _conv_s0_y, _conv_s0_z, _conv_s0_n, _input, _filter, _bias, ), name = "_conv_0") #for

    def _conv_1(conv_s1_x, conv_s1_y, conv_s1_z, conv_s1_n, _input, _filter, _bias, _conv, ):
        out = hcl.local(init = _conv[conv_s1_x, conv_s1_y, conv_s1_z, conv_s1_n])
        # for conv_s1_r__z in range(32):
        #     for conv_s1_r__y in range(3):
        #         for conv_s1_r__x in range(3):
        #             out[0] = out[0] + _filter[((conv_s1_z * 288) + ((conv_s1_r__z * 9) + ((conv_s1_r__y * 3) + conv_s1_r__x)))] * _input[((conv_s1_n * 143648) + ((conv_s1_r__z * 4489) + (((conv_s1_r__y + conv_s1_y) * 67) + (conv_s1_r__x + conv_s1_x))))]
        # return out[0]
        _sum = hcl.reducer(0, lambda x, y: x + y)
        conv_s1_r__z = hcl.reduce_axis(0, 32)
        conv_s1_r__y = hcl.reduce_axis(0, 3)
        conv_s1_r__x = hcl.reduce_axis(0, 3)
        out[0] = out[0] + _sum(
            axis = [conv_s1_r__z, conv_s1_r__y, conv_s1_r__x, ], 
            expr = _filter[((conv_s1_z * 288) + ((conv_s1_r__z * 9) + ((conv_s1_r__y * 3) + conv_s1_r__x)))] * _input[((conv_s1_n * 143648) + ((conv_s1_r__z * 4489) + (((conv_s1_r__y + conv_s1_y) * 67) + (conv_s1_r__x + conv_s1_x))))],        
        )
        return out[0]
    _conv = hcl.compute((64, 64, 32, 4, ), lambda _conv_s1_x, _conv_s1_y, _conv_s1_z, _conv_s1_n: _conv_1(_conv_s1_x, _conv_s1_y, _conv_s1_z, _conv_s1_n, _input, _filter, _bias, _conv, ), name = "_conv_1")

    def _conv_2(conv_s2_x, conv_s2_y, conv_s2_z, conv_s2_n, _input, _filter, _bias, _conv, ):
        out = hcl.local(init = _conv[conv_s2_x, conv_s2_y, conv_s2_z, conv_s2_n])
        out[0] = hcl.select(out[0] > float(0), out[0], float(0)) # out[0] = max(out[0], 0)
        return out[0]
    _conv = hcl.compute((64, 64, 32, 4, ), lambda _conv_s2_x, _conv_s2_y, _conv_s2_z, _conv_s2_n: _conv_2(_conv_s2_x, _conv_s2_y, _conv_s2_z, _conv_s2_n, _input, _filter, _bias, _conv, ), name = "_conv_2")

    return _conv #produce

_input = hcl.placeholder((67*67*32*4, ))
_filter = hcl.placeholder((3*3*32*32, ))
_bias = hcl.placeholder((32, ))

s = hcl.create_schedule([_input, _filter, _bias, ], _conv)
print(hcl.lower(s))
f = hcl.build(s)

import numpy as np
np_in = np.load("input.npy")
np_filter = np.load("filter.npy")
np_bias = np.load("bias.npy")

np_in = np.transpose(np_in, (3,2,1,0)).flatten()
np_filter = np.transpose(np_filter, (3,2,1,0)).flatten()

hcl_in = hcl.asarray(np_in)
hcl_filter = hcl.asarray(np_filter)
hcl_bias = hcl.asarray(np_bias)

out_shape = (64, 64, 32, 4) #Here just the output shape
hcl_out = hcl.asarray(np.zeros(out_shape))
f(hcl_in, hcl_filter, hcl_bias, hcl_out)

np_out = hcl_out.asnumpy()
# np_out_64 = np_out[0:64, 0:64, :, :] #don't need to slice
np.save("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl_target.npy", np_out)