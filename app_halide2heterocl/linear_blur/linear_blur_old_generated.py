import heterocl as hcl
hcl.init()
_srgb_extent_0 = 766
_srgb_extent_1 = 1278
_srgb_extent_2 = 3
def _all(_input, ):
    
    
    def _linear(_linear_s0_x, _linear_s0_y, _linear_s0_c, _input, ):
        out = hcl.local(0)
        _t12 = _input[_linear_s0_x, _linear_s0_y, _linear_s0_c]
        out[0] = hcl.select((float(0.040450) < _t12), hcl.power(((_t12 * float(0.947867)) + float(0.052133)), float(2.400000)), (_t12 * float(0.077399)))
        return out[0]
    _linear = hcl.compute(((_srgb_extent_0 + 2), (_srgb_extent_1 + 2), _srgb_extent_2, ), lambda _linear_s0_x, _linear_s0_y, _linear_s0_c, : _linear(_linear_s0_x, _linear_s0_y, _linear_s0_c, _input, ), name = "_linear")

    def _blur_x(_blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c, _linear):
        out = hcl.local(0)
        out[0] = ((_linear[(_blur_x_s0_x + 2), _blur_x_s0_y, _blur_x_s0_c] + (_linear[_blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c] + _linear[(_blur_x_s0_x + 1), _blur_x_s0_y, _blur_x_s0_c])) * float(0.333333))
        return out[0]
    _blur_x = hcl.compute((_srgb_extent_0, (_srgb_extent_1 + 2), _srgb_extent_2, ), lambda _blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c, : _blur_x(_blur_x_s0_x, _blur_x_s0_y, _blur_x_s0_c, _linear), name = "_blur_x")

    def _blur_y(_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, _blur_x):
        out = hcl.local(0)
        out[0] = ((_blur_x[_blur_y_s0_x, (_blur_y_s0_y + 2), _blur_y_s0_c] + (_blur_x[_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c] + _blur_x[_blur_y_s0_x, (_blur_y_s0_y + 1), _blur_y_s0_c])) * float(0.333333))
        return out[0]
    _blur_y = hcl.compute((_srgb_extent_0, _srgb_extent_1, _srgb_extent_2, ), lambda _blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, : _blur_y(_blur_y_s0_x, _blur_y_s0_y, _blur_y_s0_c, _blur_x), name = "_blur_y")

    def _srgb(_srgb_s0_x, _srgb_s0_y, _srgb_s0_c, _blur_y):
        out = hcl.local(0)
        _t13 = _blur_y[_srgb_s0_x, _srgb_s0_y, _srgb_s0_c]
        out[0] = hcl.select((float(0.003131) < _t13), ((hcl.power(_t13, float(0.416667)) * float(1.055000)) + float(-0.055000)), (_t13 * float(12.920000)))
        return out[0]
    _srgb = hcl.compute((_srgb_extent_0, _srgb_extent_1, _srgb_extent_2, ), lambda _srgb_s0_x, _srgb_s0_y, _srgb_s0_c, : _srgb(_srgb_s0_x, _srgb_s0_y, _srgb_s0_c, _blur_y), name = "_srgb")

    return _srgb
_input = hcl.placeholder((768 , 1280 , 3, ), name = "_input")
s = hcl.create_schedule([_input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
