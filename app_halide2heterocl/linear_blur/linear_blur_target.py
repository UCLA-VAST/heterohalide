import heterocl as hcl 
hcl.init()
_srgb_min_2 = _srgb_min_1 = _srgb_min_0 = 0
_srgb_extent_2 = 3
_srgb_extent_1 = 1278
_srgb_extent_0 = 766
_linear_stride_2 = ((_srgb_extent_0 + 2) * (_srgb_extent_1 + 2))

def linear_blur(_input): # compile def
    def _linear(_linear_s0_x, _linear_s0_y, _linear_s0_c, _input): # produce, 3*For
        out = hcl.local(0, dtype = hcl.Float()) # Provide
        _t12 = _input[_linear_s0_x, _linear_s0_y, _linear_s0_c] #Let
        out[0] = hcl.select((float(0.040450) < _t12), hcl.power(((_t12 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t12 * float(0.077399))) # Let
        return out[0] # Provide
    _linear = hcl.compute((_srgb_extent_0 + 2, _srgb_extent_1 + 2, _srgb_extent_2), lambda _linear_s0_x, _linear_s0_y, _linear_s0_c: _linear(_linear_s0_x, _linear_s0_y, _linear_s0_c, _input), name = "linear", dtype = hcl.Float()) # produce

    def _blur_x(_blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c, _linear): # produce, 3*For
        out = hcl.local(0, dtype = hcl.Float()) # Provide
        out[0] = ((_linear[(_blur_x_s0_x + 2), _blur_x_s0_y, _blur_x_s0_c] + (_linear[_blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c] + _linear[(_blur_x_s0_x + 1), _blur_x_s0_y, _blur_x_s0_c])) * float(0.333333))
        return out[0] # Provide
    _blur_x = hcl.compute((_srgb_extent_0, _srgb_extent_1 + 2, _srgb_extent_2), lambda _blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c: _blur_x(_blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c, _linear), name = "blur_x", dtype = hcl.Float())

    def _blur_y(_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, _blur_x):
        out = hcl.local(0, dtype = hcl.Float())
        out[0] = ((_blur_x[_blur_y_s0_x, (_blur_y_s0_y + 2), _blur_y_s0_c] + (_blur_x[_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c] + _blur_x[_blur_y_s0_x, (_blur_y_s0_y + 1), _blur_y_s0_c])) * float(0.333333)) # Provide + Op
        return out[0]
    _blur_y = hcl.compute((_srgb_extent_0, _srgb_extent_1, _srgb_extent_2), lambda _blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c: _blur_y(_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, _blur_x), name = "blur_y", dtype = hcl.Float())

    def _srgb(_srgb_s0_x, _srgb_s0_y, _srgb_s0_c, _blur_y):
        out = hcl.local(0, dtype = hcl.Float())
        _t13 = _blur_y[_srgb_s0_x, _srgb_s0_y, _srgb_s0_c]
        out[0] = hcl.select((float(0.003131) < _t13), ((hcl.power(_t13, float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t13 * float(12.920000)))
        return out[0]
    _srgb = hcl.compute((_srgb_extent_0, _srgb_extent_1, _srgb_extent_2), lambda _srgb_s0_x, _srgb_s0_y, _srgb_s0_c: _srgb(_srgb_s0_x, _srgb_s0_y, _srgb_s0_c, _blur_y), name = "srgb", dtype = hcl.Float())

    return _srgb # compile def

_input = hcl.placeholder((768, 1280, 3, ), name = "_input", dtype = hcl.Float())
s = hcl.create_schedule([_input], linear_blur)
print(hcl.lower(s))
f = hcl.build(s)

import numpy as np 
np_in = np.load("input.npy")
hcl_in = hcl.asarray(np_in, dtype = hcl.Float())
output_shape = (766, 1278, 3)
hcl_out = hcl.asarray(np.zeros(output_shape).astype('float32'), dtype = hcl.Float())
# need to specify the data type of the HeteroCL array instead of numpy array, otherwise there will be data type error


f(hcl_in, hcl_out)
print('execute done')

np_out = hcl_out.asnumpy()
np.save('output_heterocl_new_target.npy', np_out)