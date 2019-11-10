import heterocl as hcl
hcl.init(hcl.Float())
_srgb_extent_0 = 766
_srgb_extent_1 = 1278
_srgb_extent_2 = 3
_srgb_min_0 = 0
_srgb_min_1 = 0
_srgb_min_2 = 0
def _all(_input, ):
    def _linear_0(_linear_s0_c, _linear_s0_x, _linear_s0_y, _input, ):
        out = hcl.local(0)
        _t8 = _input[_linear_s0_c, _linear_s0_y, _linear_s0_x]
        out[0] = hcl.select((float(0.040450) < _t8), hcl.power(((_t8 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t8 * float(0.077399)))
        return out[0]
    _linear_0 = hcl.compute((_srgb_extent_2, (_srgb_extent_0 + 2), (_srgb_extent_1 + 2), ), lambda _linear_s0_c, _linear_s0_x, _linear_s0_y, : _linear_0(_linear_s0_c, _linear_s0_x, _linear_s0_y, _input, ), name = "_linear_0")

    def _blur_x_0(_blur_x_s0_y, _blur_x_s0_x, _blur_x_s0_c, _input, _linear, ):
        out = hcl.local(0)
        out[0] = ((_linear[_blur_x_s0_c, (_blur_x_s0_x + 2), _blur_x_s0_y] + (_linear[_blur_x_s0_c, _blur_x_s0_x, _blur_x_s0_y] + _linear[_blur_x_s0_c, (_blur_x_s0_x + 1), _blur_x_s0_y])) * float(0.333333))
        return out[0]
    _blur_x_0 = hcl.compute(((_srgb_extent_1 + 2), _srgb_extent_0, _srgb_extent_2, ), lambda _blur_x_s0_y, _blur_x_s0_x, _blur_x_s0_c, : _blur_x_0(_blur_x_s0_y, _blur_x_s0_x, _blur_x_s0_c, _input, _linear_0, ), name = "_blur_x_0")

    def _blur_y_0(_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, _input, _blur_x, ):
        out = hcl.local(0)
        out[0] = ((_blur_x[(_blur_y_s0_y + 2), _blur_y_s0_x, _blur_y_s0_c] + (_blur_x[_blur_y_s0_y, _blur_y_s0_x, _blur_y_s0_c] + _blur_x[(_blur_y_s0_y + 1), _blur_y_s0_x, _blur_y_s0_c])) * float(0.333333))
        return out[0]
    _blur_y_0 = hcl.compute((_srgb_extent_0, _srgb_extent_1, _srgb_extent_2, ), lambda _blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, : _blur_y_0(_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, _input, _blur_x_0, ), name = "_blur_y_0")

    def _srgb_0(_srgb_s0_c, _srgb_s0_y, _srgb_s0_x, _input, _blur_y, ):
        out = hcl.local(0)
        _t9 = _blur_y[_srgb_s0_x, _srgb_s0_y, _srgb_s0_c]
        out[0] = hcl.select((float(0.003131) < _t9), ((hcl.power(_t9, float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t9 * float(12.920000)))
        return out[0]
    _srgb_0 = hcl.compute((_srgb_extent_2, _srgb_extent_1, _srgb_extent_0, ), lambda _srgb_s0_c, _srgb_s0_y, _srgb_s0_x, : _srgb_0(_srgb_s0_c, _srgb_s0_y, _srgb_s0_x, _input, _blur_y_0, ), name = "_srgb_0")

    return _srgb_0
_input = hcl.placeholder((3, 1280, 768, ), name = "_input")
s = hcl.create_schedule([_input, ], _all)
s_blur_x_0 = _all._blur_x_0
s[s_blur_x_0].unroll(s_blur_x_0.axis[2], 3)
s_blur_y_0 = _all._blur_y_0
s[s_blur_y_0].unroll(s_blur_y_0.axis[0], 15)
s_linear_0 = _all._linear_0
s[s_linear_0].unroll(s_linear_0.axis[1], 8)
s_srgb_0 = _all._srgb_0
s[s_srgb_0].unroll(s_srgb_0.axis[0], 7)
s[s_srgb_0].unroll(s_srgb_0.axis[1], 10)
s[s_srgb_0].unroll(s_srgb_0.axis[2], 2)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("_input.npy"), (2, 1, 0))
hcl_input = hcl.asarray(np_input)
output_shape = (3, 1278, 766)
hcl_out = hcl.asarray(np.zeros(output_shape))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (2, 1, 0))
