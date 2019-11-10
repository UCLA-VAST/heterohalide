import heterocl as hcl
hcl.init()
_output_extent_0 = 24
_output_extent_1 = 24
_output_extent_2 = 3
_output_min_0 = 0
_output_min_1 = 0
_output_min_2 = 0
def _all(_input, ):
    def _gray_0(_gray_s0_y, _gray_s0_x, _input, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (((_input[2, _gray_s0_y, _gray_s0_x] * 29) + ((_input[0, _gray_s0_y, _gray_s0_x] * 77) + (_input[1, _gray_s0_y, _gray_s0_x] * 150)))/256))
    _gray_0 = hcl.compute(((_output_extent_1 + 8), (_output_extent_0 + 8), ), lambda _gray_s0_y, _gray_s0_x, : _gray_0(_gray_s0_y, _gray_s0_x, _input, ), name = "_gray_0", dtype = hcl.UInt(bits = 8))

    def _sum_x_0(_sum_x_s0_y, _sum_x_s0_x, _input, ):
        return 0
    _sum_x_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _sum_x_s0_y, _sum_x_s0_x, : _sum_x_0(_sum_x_s0_y, _sum_x_s0_x, _input, ), name = "_sum_x_0", dtype = hcl.UInt(bits = 32))

    def _sum_x_1(_sum_x_s1_y, _sum_x_s1_x, _input, _sum_x, _gray, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _sum_x_s1_win2__x = hcl.reduce_axis(0, 9)
        _sum_x_s1_win2__y = hcl.reduce_axis(0, 9)
        return (_sum_x[_sum_x_s1_y, _sum_x_s1_x] + _sum(
            axis = [_sum_x_s1_win2__y, _sum_x_s1_win2__x, ],
            expr = ((hcl.cast(dtype = hcl.UInt(bits = 32), expr = _gray[(_sum_x_s1_win2__y + _sum_x_s1_y), (_sum_x_s1_win2__x + _sum_x_s1_x)]) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - _sum_x_s1_win2__x) * (_sum_x_s1_win2__x + -4))) * float(0.222222))) * float(67.972794))))) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - _sum_x_s1_win2__y) * (_sum_x_s1_win2__y + -4))) * float(0.222222))) * float(67.972794)))))
        ))
    _sum_x_1 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _sum_x_s1_y, _sum_x_s1_x, : _sum_x_1(_sum_x_s1_y, _sum_x_s1_x, _input, _sum_x_0, _gray_0, ), name = "_sum_x_1", dtype = hcl.UInt(bits = 32))

    def _blur_x_0(_blur_x_s0_y, _blur_x_s0_x, _input, _sum_x, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (_sum_x[_blur_x_s0_y, _blur_x_s0_x]/65536))
    _blur_x_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _blur_x_s0_y, _blur_x_s0_x, : _blur_x_0(_blur_x_s0_y, _blur_x_s0_x, _input, _sum_x_1, ), name = "_blur_x_0", dtype = hcl.UInt(bits = 8))

    def _sharpen_0(_sharpen_s0_y, _sharpen_s0_x, _input, _blur_x, _gray, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = _gray[_sharpen_s0_y, _sharpen_s0_x]) * 2) - hcl.cast(dtype = hcl.Int(bits = 16), expr = _blur_x[_sharpen_s0_y, _sharpen_s0_x])) < 255, ((hcl.cast(dtype = hcl.Int(bits = 16), expr = _gray[_sharpen_s0_y, _sharpen_s0_x]) * 2) - hcl.cast(dtype = hcl.Int(bits = 16), expr = _blur_x[_sharpen_s0_y, _sharpen_s0_x])), 255) > 0, hcl.select(((hcl.cast(dtype = hcl.Int(bits = 16), expr = _gray[_sharpen_s0_y, _sharpen_s0_x]) * 2) - hcl.cast(dtype = hcl.Int(bits = 16), expr = _blur_x[_sharpen_s0_y, _sharpen_s0_x])) < 255, ((hcl.cast(dtype = hcl.Int(bits = 16), expr = _gray[_sharpen_s0_y, _sharpen_s0_x]) * 2) - hcl.cast(dtype = hcl.Int(bits = 16), expr = _blur_x[_sharpen_s0_y, _sharpen_s0_x])), 255), 0))
    _sharpen_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _sharpen_s0_y, _sharpen_s0_x, : _sharpen_0(_sharpen_s0_y, _sharpen_s0_x, _input, _blur_x_0, _gray_0, ), name = "_sharpen_0", dtype = hcl.UInt(bits = 8))

    def _ratio_0(_ratio_s0_y, _ratio_s0_x, _input, _sharpen, _gray, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = _sharpen[_ratio_s0_y, _ratio_s0_x]) * 32)/hcl.cast(dtype = hcl.Int(bits = 16), expr = hcl.select(_gray[_ratio_s0_y, _ratio_s0_x] > 1, _gray[_ratio_s0_y, _ratio_s0_x], 1))) < 255, ((hcl.cast(dtype = hcl.Int(bits = 16), expr = _sharpen[_ratio_s0_y, _ratio_s0_x]) * 32)/hcl.cast(dtype = hcl.Int(bits = 16), expr = hcl.select(_gray[_ratio_s0_y, _ratio_s0_x] > 1, _gray[_ratio_s0_y, _ratio_s0_x], 1))), 255) > 0, hcl.select(((hcl.cast(dtype = hcl.Int(bits = 16), expr = _sharpen[_ratio_s0_y, _ratio_s0_x]) * 32)/hcl.cast(dtype = hcl.Int(bits = 16), expr = hcl.select(_gray[_ratio_s0_y, _ratio_s0_x] > 1, _gray[_ratio_s0_y, _ratio_s0_x], 1))) < 255, ((hcl.cast(dtype = hcl.Int(bits = 16), expr = _sharpen[_ratio_s0_y, _ratio_s0_x]) * 32)/hcl.cast(dtype = hcl.Int(bits = 16), expr = hcl.select(_gray[_ratio_s0_y, _ratio_s0_x] > 1, _gray[_ratio_s0_y, _ratio_s0_x], 1))), 255), 0))
    _ratio_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _ratio_s0_y, _ratio_s0_x, : _ratio_0(_ratio_s0_y, _ratio_s0_x, _input, _sharpen_0, _gray_0, ), name = "_ratio_0", dtype = hcl.UInt(bits = 8))

    def _output_0(_output_s0_c, _output_s0_y, _output_s0_x, _input, _ratio, ):
        return hcl.select(((_input[_output_s0_c, (_output_s0_y + 4), (_output_s0_x + 4)] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = _ratio[_output_s0_y, _output_s0_x]))/32) < 255, ((_input[_output_s0_c, (_output_s0_y + 4), (_output_s0_x + 4)] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = _ratio[_output_s0_y, _output_s0_x]))/32), 255)
    _output_0 = hcl.compute((_output_extent_2, _output_extent_1, _output_extent_0, ), lambda _output_s0_c, _output_s0_y, _output_s0_x, : _output_0(_output_s0_c, _output_s0_y, _output_s0_x, _input, _ratio_0, ), name = "_output_0", dtype = hcl.UInt(bits = 8))

    return _output_0
_input = hcl.placeholder((3, 30, 30, ), name = "_input")
s = hcl.create_schedule([_input, ], _all)
# f = hcl.build(s)
print(hcl.lower(s))
# import numpy as np
# np_input = np.transpose(np.load("input.npy"), (2, 1, 0))
# hcl_input = hcl.asarray(np_input)
# output_shape = (3, 24, 24)
# hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
# f(hcl_input, hcl_out)
# np_out = hcl_out.asnumpy()
# np_out = np.transpose(np_out, (2, 1, 0))
# np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
# print(hcl.build(s, target = "merlinc"))