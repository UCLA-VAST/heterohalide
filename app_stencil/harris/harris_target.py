import heterocl as hcl
hcl.init()
_output_extent_0 = 8
_output_extent_1 = 8
_output_min_0 = 0
_output_min_1 = 0
def _all(_b0, ):
    def _padded16_0(_padded16_s0_y, _padded16_s0_x, _b0, ):
        return (_b0[_padded16_s0_y, _padded16_s0_x]) #cast like int16 don't exist in python. Might displat like 
    _padded16_0 = hcl.compute(((_output_extent_1 + 6), (_output_extent_0 + 6), ), lambda _padded16_s0_y, _padded16_s0_x, : _padded16_0(_padded16_s0_y, _padded16_s0_x, _b0, ), name = "_padded16_0", dtype = hcl.Int(bits = 16))

    def _grad_x_0(_grad_x_s0_y, _grad_x_s0_x, _b0, _padded16, ):
        return (_padded16[(_grad_x_s0_y + 2), (_grad_x_s0_x + 2)] + (((_padded16[(_grad_x_s0_y + 1), (_grad_x_s0_x + 2)] * 2) + ((_padded16[_grad_x_s0_y, (_grad_x_s0_x + 2)] - _padded16[_grad_x_s0_y, _grad_x_s0_x]) - (_padded16[(_grad_x_s0_y + 1), _grad_x_s0_x] * 2))) - _padded16[(_grad_x_s0_y + 2), _grad_x_s0_x]))
    _grad_x_0 = hcl.compute(((_output_extent_1 + 4), (_output_extent_0 + 4), ), lambda _grad_x_s0_y, _grad_x_s0_x, : _grad_x_0(_grad_x_s0_y, _grad_x_s0_x, _b0, _padded16_0, ), name = "_grad_x_0", dtype = hcl.Int(bits = 16))

    def _grad_xx_0(_grad_xx_s0_y, _grad_xx_s0_x, _b0, _grad_x, ):
        _t71_s = _grad_x[_grad_xx_s0_y, _grad_xx_s0_x]
        return ((_t71_s) * (_t71_s)) #what's this bug? out[0] but no return. Let Stmt... Let... in ...
    _grad_xx_0 = hcl.compute(((_output_extent_1 + 4), (_output_extent_0 + 4), ), lambda _grad_xx_s0_y, _grad_xx_s0_x, : _grad_xx_0(_grad_xx_s0_y, _grad_xx_s0_x, _b0, _grad_x_0, ), name = "_grad_xx_0", dtype = hcl.Int(bits = 32))

    def _grad_gx_0(_grad_gx_s0_y, _grad_gx_s0_x, _b0, _grad_xx, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _grad_gx_s0_box__y = hcl.reduce_axis(0, 3) # symbol to judge if it's reduction domain need to be "$" instead of "r"
        _grad_gx_s0_box__x = hcl.reduce_axis(0, 3)
        return _sum(
            axis = [_grad_gx_s0_box__y, _grad_gx_s0_box__x, ], 
            expr = _grad_xx[(_grad_gx_s0_box__y + _grad_gx_s0_y), (_grad_gx_s0_box__x + _grad_gx_s0_x)]
        )

    _grad_gx_0 = hcl.compute(((_output_extent_1 + 2), (_output_extent_0 + 2), ), lambda _grad_gx_s0_y, _grad_gx_s0_x, : _grad_gx_0(_grad_gx_s0_y, _grad_gx_s0_x, _b0, _grad_xx_0, ), name = "_grad_gx_0", dtype = hcl.Int(bits = 32))

    def _grad_y_0(_grad_y_s0_y, _grad_y_s0_x, _b0, _padded16, ): # bug here: got wrong about the last stage's shape
        return ((_padded16[(_grad_y_s0_y + 2), (_grad_y_s0_x + 2)] + (((_padded16[(_grad_y_s0_y + 2), (_grad_y_s0_x + 1)] * 2) + (_padded16[(_grad_y_s0_y + 2), _grad_y_s0_x] - _padded16[_grad_y_s0_y, _grad_y_s0_x])) - (_padded16[_grad_y_s0_y, (_grad_y_s0_x + 1)] * 2))) - _padded16[_grad_y_s0_y, (_grad_y_s0_x + 2)])
    _grad_y_0 = hcl.compute(((_output_extent_1 + 4), (_output_extent_0 + 4), ), lambda _grad_y_s0_y, _grad_y_s0_x, : _grad_y_0(_grad_y_s0_y, _grad_y_s0_x, _b0, _padded16_0, ), name = "_grad_y_0", dtype = hcl.Int(bits = 16))

    def _grad_xy_0(_grad_xy_s0_y, _grad_xy_s0_x, _b0, _grad_y, _grad_x ): # bug here: use grad_x but didn't show it in definition(...). Need a map maybe
        return ((_grad_x[_grad_xy_s0_y, _grad_xy_s0_x]) * (_grad_y[_grad_xy_s0_y, _grad_xy_s0_x]))
    _grad_xy_0 = hcl.compute(((_output_extent_1 + 4), (_output_extent_0 + 4), ), lambda _grad_xy_s0_y, _grad_xy_s0_x, : _grad_xy_0(_grad_xy_s0_y, _grad_xy_s0_x, _b0, _grad_y_0, _grad_x_0, ), name = "_grad_xy_0", dtype = hcl.Int(bits = 32))

    def _grad_gxy_0(_grad_gxy_s0_y, _grad_gxy_s0_x, _b0, _grad_xy, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _grad_gxy_s0_box__y = hcl.reduce_axis(0, 3)
        _grad_gxy_s0_box__x = hcl.reduce_axis(0, 3)
        return _sum(
            axis = [_grad_gxy_s0_box__y, _grad_gxy_s0_box__x], 
            expr = _grad_xy[(_grad_gxy_s0_box__y + _grad_gxy_s0_y), (_grad_gxy_s0_box__x + _grad_gxy_s0_x)]
        )
    _grad_gxy_0 = hcl.compute(((_output_extent_1 + 2), (_output_extent_0 + 2), ), lambda _grad_gxy_s0_y, _grad_gxy_s0_x, : _grad_gxy_0(_grad_gxy_s0_y, _grad_gxy_s0_x, _b0, _grad_xy_0, ), name = "_grad_gxy_0", dtype = hcl.Int(bits = 32))

    def _grad_yy_0(_grad_yy_s0_y, _grad_yy_s0_x, _b0, _grad_y, ):
        _t72_s = _grad_y[_grad_yy_s0_y, _grad_yy_s0_x]
        return ((_t72_s) * (_t72_s))
    _grad_yy_0 = hcl.compute(((_output_extent_1 + 4), (_output_extent_0 + 4), ), lambda _grad_yy_s0_y, _grad_yy_s0_x, : _grad_yy_0(_grad_yy_s0_y, _grad_yy_s0_x, _b0, _grad_y_0, ), name = "_grad_yy_0", dtype = hcl.Int(bits = 32))

    def _grad_gy_0(_grad_gy_s0_y, _grad_gy_s0_x, _b0, _grad_yy, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        _grad_gy_s0_box__y = hcl.reduce_axis(0, 3)
        _grad_gy_s0_box__x = hcl.reduce_axis(0, 3)
        return _sum(
            axis = [_grad_gy_s0_box__y, _grad_gy_s0_box__x, ], 
            expr = _grad_yy[(_grad_gy_s0_box__y + _grad_gy_s0_y), (_grad_gy_s0_box__x + _grad_gy_s0_x)]
        )
    _grad_gy_0 = hcl.compute(((_output_extent_1 + 2), (_output_extent_0 + 2), ), lambda _grad_gy_s0_y, _grad_gy_s0_x, : _grad_gy_0(_grad_gy_s0_y, _grad_gy_s0_x, _b0, _grad_yy_0, ), name = "_grad_gy_0", dtype = hcl.Int(bits = 32))

    def _cim_0(_cim_s0_y, _cim_s0_x, _b0, _grad_gy, _grad_gx, _grad_gxy ):# bug: last stage's dimension / shape
        _t73 = _grad_gx[_cim_s0_y, _cim_s0_x]
        _t74 = _grad_gy[_cim_s0_y, _cim_s0_x]
        _t75 = _grad_gxy[_cim_s0_y, _cim_s0_x]
        _t76 = (hcl.cast(dtype = hcl.Float(), expr = (_t73 / 144)) + hcl.cast(dtype = hcl.Float(), expr = (_t74/144))) # float cant used to tensor/intermediate variable, only can be used to number
        return (((hcl.cast(dtype = hcl.Float(), expr = (_t73 / 144)) * hcl.cast(dtype = hcl.Float(), expr = (_t74/144))) - (hcl.cast(dtype = hcl.Float(), expr = (_t75/144)) * hcl.cast(dtype = hcl.Float(), expr = (_t75/144)))) - ((_t76 * _t76) * (0.040000)))
    _cim_0 = hcl.compute(((_output_extent_1 + 2), (_output_extent_0 + 2), ), lambda _cim_s0_y, _cim_s0_x, : _cim_0(_cim_s0_y, _cim_s0_x, _b0, _grad_gy_0, _grad_gx_0, _grad_gxy_0, ), name = "_cim_0", dtype = hcl.Float())

    def _hw_ouput_0(_hw_ouput_s0_y, _hw_ouput_s0_x, _b0, _cim, ):
        _t77 = _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 1)] # can't use "and".   100.00000 <= _t77 seems not to be a Expr but an Array? Cause Error
        return hcl.select(hcl.and_(((100.000000) <= _t77), (hcl.select(_cim[(_hw_ouput_s0_y + 2), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 2), (_hw_ouput_s0_x + 1)] > hcl.select(_cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x] > hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]))), _cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x], hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])))), _cim[(_hw_ouput_s0_y + 2), (_hw_ouput_s0_x + 1)], hcl.select(_cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x] > hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]))), _cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x], hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]))))), _cim[(_hw_ouput_s0_y + 2), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 2), (_hw_ouput_s0_x + 1)] > hcl.select(_cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x] > hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]))), _cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x], hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])))), _cim[(_hw_ouput_s0_y + 2), (_hw_ouput_s0_x + 1)], hcl.select(_cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x] > hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]))), _cim[(_hw_ouput_s0_y + 2), _hw_ouput_s0_x], hcl.select(_cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)] > hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])), _cim[(_hw_ouput_s0_y + 1), (_hw_ouput_s0_x + 2)], hcl.select(_cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x] > hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)]), _cim[(_hw_ouput_s0_y + 1), _hw_ouput_s0_x], hcl.select(_cim[_hw_ouput_s0_y, _hw_ouput_s0_x] > _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)], _cim[_hw_ouput_s0_y, _hw_ouput_s0_x], _cim[_hw_ouput_s0_y, (_hw_ouput_s0_x + 1)])))))) < _t77)), 255, 0)
    _hw_ouput_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _hw_ouput_s0_y, _hw_ouput_s0_x, : _hw_ouput_0(_hw_ouput_s0_y, _hw_ouput_s0_x, _b0, _cim_0, ), name = "_hw_ouput_0", dtype = hcl.UInt(bits = 8))

    def _output_0(_output_s0_y, _output_s0_x, _b0, _hw_ouput, ):
        return _hw_ouput[_output_s0_y, _output_s0_x]
    _output_0 = hcl.compute((_output_extent_1, _output_extent_0, ), lambda _output_s0_y, _output_s0_x, : _output_0(_output_s0_y, _output_s0_x, _b0, _hw_ouput_0, ), name = "_output_0", dtype = hcl.UInt(bits = 8))

    return _output_0

_b0 = hcl.placeholder((14, 14, ), name = "_b0", dtype = hcl.UInt(bits = 8))
s = hcl.create_schedule([_b0, ], _all)
print("create schedule success")
f = hcl.build(s)
print("build success")
print(hcl.lower(s))
import numpy as np
# import cv2 
# np_b0 = cv2.imread("/curr/jiajieli/Halide-HLS/apps/images/benchmark_8mp_gray.png")
# print(np_b0.shape)
# np_b0 = np.transpose(np_b0[:,:,0])
np_b0 = np.transpose(np.load("input.npy"), (1, 0))
hcl_b0 = hcl.asarray(np_b0, dtype = hcl.UInt(bits = 8))
output_shape = (8, 8)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(8)) #dtype = hcl.UInt(bits = 8))
f(hcl_b0, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl.npy", np_out)
# cv2.imwrite("out_hcl.png", np_out)