import heterocl as hcl
hcl.init()
_output_extent_0 = 629
_output_extent_1 = 397
_output_min_0 = 0
_output_min_1 = 0
def _all(_left, _right, ):
    def _SAD_0(_SAD_s0_c, _SAD_s0_y, _SAD_s0_x, _left, _right, ):
        return 0
    _SAD_0 = hcl.compute((64, _output_extent_1, _output_extent_0, ), lambda _SAD_s0_c, _SAD_s0_y, _SAD_s0_x, : _SAD_0(_SAD_s0_c, _SAD_s0_y, _SAD_s0_x, _left, _right, ), name = "_SAD_0", dtype = hcl.UInt(bits = 16))

    def _SAD_1(_SAD_s1_c, _SAD_s1_y, _SAD_s1_x, _left, _right, _SAD, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _SAD_s1_win__x = hcl.reduce_axis(0, 8)
        _SAD_s1_win__y = hcl.reduce_axis(0, 8)
        return (_SAD[_SAD_s1_x, _SAD_s1_y, _SAD_s1_x] + _sum(
            axis = [_SAD_s1_win__y, _SAD_s1_win__x, ],
            expr = absd(_right[(_SAD_s1_win__y + _SAD_s1_y), (_SAD_s1_win__x + _SAD_s1_x)], _left[(_SAD_s1_win__y + _SAD_s1_y), (((_SAD_s1_win__x + _SAD_s1_x) + _SAD_s1_c) + 20)], 
        ))
    _SAD_1 = hcl.compute((64, _output_extent_1, _output_extent_0, ), lambda _SAD_s1_c, _SAD_s1_y, _SAD_s1_x, : _SAD_1(_SAD_s1_c, _SAD_s1_y, _SAD_s1_x, _left, _right, _SAD_0, ), name = "_SAD_1", dtype = hcl.UInt(bits = 16))

    def _offset_0(_offset_s0_y, _offset_s0_x, _left, _right, ): # Bugs here. Can't deal with 2 provide visitor in one for loop......
        return 0
        return 65535
    _offset_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _offset_s0_y, _offset_s0_x, : _offset_0(_offset_s0_y, _offset_s0_x, _left, _right, ), name = "_offset_0", dtype = hcl.UInt(bits = 16))

    hcl.compute()
    _sum = hcl.reducer(0, lambda x, y: x + y)
    _offset_s1_search__x = hcl.reduce_axis(0, 64)
    return hcl.select((_SAD[_offset_s1_y, _offset_s1_x] < _offset_1[_offset_s1_y, _offset_s1_x]), hcl.cast(dtype = hcl.Int(bits = 8), expr = _offset_s1_search__x), _offset_0[_offset_s1_y, _offset_s1_x])
    _sum = hcl.reducer(0, lambda x, y: x + y)
    _offset_s1_search__x = hcl.reduce_axis(0, 64)
    return hcl.select(_SAD[_offset_s1_y, _offset_s1_x] < _offset_1[_offset_s1_y, _offset_s1_x], _SAD[_offset_s1_y, _offset_s1_x], _offset_1[_offset_s1_y, _offset_s1_x])

    def _hw_output_0(_offset_s1_y, _offset_s1_x, _hw_output_s0_y, _hw_output_s0_x, _left, _right, _offset, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _offset_s1_search__x = hcl.reduce_axis(0, 64)
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = _offset_0[_hw_output_s0_y, _hw_output_s0_x]) * _sum(
            axis = [_offset_s1_search__x, ],
            expr = 255
        ))/64))
    _hw_output_0 = hcl.compute((_output_extent_1, _output_extent_0, _output_extent_1, _output_extent_0, ), lambda _offset_s1_y, _offset_s1_x, _hw_output_s0_y, _hw_output_s0_x, : _hw_output_0(_offset_s1_y, _offset_s1_x, _hw_output_s0_y, _hw_output_s0_x, _left, _right, _offset_0, ), name = "_hw_output_0", dtype = hcl.UInt(bits = 8))

    def _output_0(_output_s0_y, _output_s0_x, _left, _right, _hw_output, ):
        return _hw_output[_output_s0_y, _output_s0_x, _output_s0_y, _output_s0_x]
    _output_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _output_s0_y, _output_s0_x, : _output_0(_output_s0_y, _output_s0_x, _left, _right, _hw_output_0, ), name = "_output_0", dtype = hcl.UInt(bits = 8))

    return _output_0
_left = hcl.placeholder((405, 720, ), name = "_left")
_right = hcl.placeholder((405, 720, ), name = "_right")
s = hcl.create_schedule([_left, _right, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_left = np.transpose(np.load("_left.npy"), (1, 0))
hcl_left = hcl.asarray(np_left)
np_right = np.transpose(np.load("_right.npy"), (1, 0))
hcl_right = hcl.asarray(np_right)
output_shape = (397, 629)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
f(hcl_left, hcl_right, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
