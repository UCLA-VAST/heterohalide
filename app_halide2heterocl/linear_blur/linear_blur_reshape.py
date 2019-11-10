import heterocl as hcl
hcl.init(hcl.Float())
_srgb_extent_0 = 766
_srgb_extent_1 = 1278
_srgb_extent_2 = 3
_srgb_min_0 = 0
_srgb_min_1 = 0
_srgb_min_2 = 0
def _all(_input, ):
    def _linear_0(_linear_s0_c, _linear_s0_y, _linear_s0_x, _input, ):
        out = hcl.local(0)
        _t16 = _input[_linear_s0_c, _linear_s0_y, _linear_s0_x]
        out[0] = hcl.select((float(0.040450) < _t16), hcl.power(((_t16 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t16 * float(0.077399)))
        return out[0]
    _linear_0 = hcl.compute((_srgb_extent_2, (_srgb_extent_1 + 2), (_srgb_extent_0 + 2), ), lambda _linear_s0_c, _linear_s0_y, _linear_s0_x, : _linear_0(_linear_s0_c, _linear_s0_y, _linear_s0_x, _input, ), name = "_linear_0")

    def _blur_x_0(_blur_x_s0_c, _blur_x_s0_y_y_outer, _blur_x_s0_y_y_inner, _blur_x_s0_x, _input, _linear, ):
        out = hcl.local(0)
        out[0] = ((_linear[_blur_x_s0_c, ((hcl.select((_blur_x_s0_y_y_outer * 640) < (_srgb_extent_1 + -638), (_blur_x_s0_y_y_outer * 640), (_srgb_extent_1 + -638)) + _srgb_min_1) + _blur_x_s0_y_y_inner), (_blur_x_s0_x + 2)] + (_linear[_blur_x_s0_c, ((hcl.select((_blur_x_s0_y_y_outer * 640) < (_srgb_extent_1 + -638), (_blur_x_s0_y_y_outer * 640), (_srgb_extent_1 + -638)) + _srgb_min_1) + _blur_x_s0_y_y_inner), _blur_x_s0_x] + _linear[_blur_x_s0_c, ((hcl.select((_blur_x_s0_y_y_outer * 640) < (_srgb_extent_1 + -638), (_blur_x_s0_y_y_outer * 640), (_srgb_extent_1 + -638)) + _srgb_min_1) + _blur_x_s0_y_y_inner), (_blur_x_s0_x + 1)])) * float(0.333333))
        return out[0]
    _blur_x_0 = hcl.compute((_srgb_extent_2, ((_srgb_extent_1 + 641)/640), 640, _srgb_extent_0, ), lambda _blur_x_s0_c, _blur_x_s0_y_y_outer, _blur_x_s0_y_y_inner, _blur_x_s0_x, : _blur_x_0(_blur_x_s0_c, _blur_x_s0_y_y_outer, _blur_x_s0_y_y_inner, _blur_x_s0_x, _input, _linear_0, ), name = "_blur_x_0")
    _blur_x_0 = hcl.compute((_srgb_extent_2, _srgb_extent_1 + 2, _srgb_extent_0,), lambda _blur_x_s0_c, _blur_x_s0_y, _blur_x_s0_x, : _blur_x_0[_blur_x_s0_c, _blur_x_s0_y, 0, _blur_x_s0_x], name = "_blur_x_0_reshape")

    def _blur_y_0(_blur_y_s0_c, _blur_y_s0_y, _blur_y_s0_x_x_outer, _blur_y_s0_x_x_inner, _input, _blur_x, ):
        out = hcl.local(0)
        out[0] = (_blur_x[_blur_y_s0_c, _blur_y_s0_y, ((hcl.select((_blur_y_s0_x_x_outer * 383) < (_srgb_extent_0 + -383), (_blur_y_s0_x_x_outer * 383), (_srgb_extent_0 + -383)) + _srgb_min_0) + _blur_y_s0_x_x_inner)] + _blur_x[_blur_y_s0_c, _blur_y_s0_y + 1, ((hcl.select((_blur_y_s0_x_x_outer * 383) < (_srgb_extent_0 + -383), (_blur_y_s0_x_x_outer * 383), (_srgb_extent_0 + -383)) + _srgb_min_0) + _blur_y_s0_x_x_inner)] + _blur_x[_blur_y_s0_c, _blur_y_s0_y + 2, ((hcl.select((_blur_y_s0_x_x_outer * 383) < (_srgb_extent_0 + -383), (_blur_y_s0_x_x_outer * 383), (_srgb_extent_0 + -383)) + _srgb_min_0) + _blur_y_s0_x_x_inner)] ) * float(0.333333)
        return out[0]
    _blur_y_0 = hcl.compute((_srgb_extent_2, _srgb_extent_1, ((_srgb_extent_0 + 382)/383), 383, ), lambda _blur_y_s0_c, _blur_y_s0_y, _blur_y_s0_x_x_outer, _blur_y_s0_x_x_inner, : _blur_y_0(_blur_y_s0_c, _blur_y_s0_y, _blur_y_s0_x_x_outer, _blur_y_s0_x_x_inner, _input, _blur_x_0, ), name = "_blur_y_0")
    _blur_y_0 = hcl.compute((_srgb_extent_2, _srgb_extent_1, _srgb_extent_0, ), lambda _blur_y_s0_c, _blur_y_s0_y, _blur_y_s0_x, : _blur_y_0[_blur_y_s0_c, 0, _blur_y_s0_y, _blur_y_s0_x], name = "_blur_y_0_reshape")

    def _srgb_0(_srgb_s0_c, _srgb_s0_y, _srgb_s0_x, _input, _blur_y, ):
        out = hcl.local(0)
        _t19 = _blur_y[_srgb_s0_c, _srgb_s0_y, _srgb_s0_x]
        out[0] = hcl.select((float(0.003131) < _t19), ((hcl.power(_t19, float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t19 * float(12.920000)))
        return out[0]
    _srgb_0 = hcl.compute((_srgb_extent_2, _srgb_extent_1, _srgb_extent_0, ), lambda _srgb_s0_c, _srgb_s0_y, _srgb_s0_x, : _srgb_0(_srgb_s0_c, _srgb_s0_y, _srgb_s0_x, _input, _blur_y_0, ), name = "_srgb_0")

    return _srgb_0
_input = hcl.placeholder((3, 1280, 768, ), name = "_input")
s = hcl.create_schedule([_input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (2, 1, 0))
hcl_input = hcl.asarray(np_input)
output_shape = (3, 1278, 766)
hcl_out = hcl.asarray(np.zeros(output_shape))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (2, 1, 0))
np.save('output_heterocl_split_generate.npy', np_out)
