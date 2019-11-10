import heterocl as hcl
hcl.init()
output_extent_0 = 3994
output_extent_1 = 5994
output_min_0 = 0
output_min_1 = 0
def _all(input, ):
    def padded16_0(padded16_s0_y, padded16_s0_x, input, ):
        return hcl.cast(dtype = hcl.Int(bits = 16), expr = input[padded16_s0_y, padded16_s0_x])
    padded16_0 = hcl.compute(((output_extent_1 + 6), (output_extent_0 + 6), ), lambda padded16_s0_y, padded16_s0_x, : padded16_0(padded16_s0_y, padded16_s0_x, input, ), name = "padded16_0", dtype = hcl.Int(bits = 16))

    def grad_x_0(grad_x_s0_y, grad_x_s0_x, input, padded16, ):
        return (padded16[(grad_x_s0_y + 2), (grad_x_s0_x + 2)] + (((padded16[(grad_x_s0_y + 1), (grad_x_s0_x + 2)] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)) + ((padded16[grad_x_s0_y, (grad_x_s0_x + 2)] - padded16[grad_x_s0_y, grad_x_s0_x]) - (padded16[(grad_x_s0_y + 1), grad_x_s0_x] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)))) - padded16[(grad_x_s0_y + 2), grad_x_s0_x]))
    grad_x_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_x_s0_y, grad_x_s0_x, : grad_x_0(grad_x_s0_y, grad_x_s0_x, input, padded16_0, ), name = "grad_x_0", dtype = hcl.Int(bits = 16))

    def grad_xx_0(grad_xx_s0_y, grad_xx_s0_x, input, grad_x, ):
        t71_s = grad_x[grad_xx_s0_y, grad_xx_s0_x]
        return (hcl.cast(dtype = hcl.Int(bits = 32), expr = t71_s) * hcl.cast(dtype = hcl.Int(bits = 32), expr = t71_s))
    grad_xx_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_xx_s0_y, grad_xx_s0_x, : grad_xx_0(grad_xx_s0_y, grad_xx_s0_x, input, grad_x_0, ), name = "grad_xx_0", dtype = hcl.Int(bits = 32))

    def grad_gx_0(grad_gx_s0_y, grad_gx_s0_x, input, ):
        return 0
    grad_gx_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gx_s0_y, grad_gx_s0_x, : grad_gx_0(grad_gx_s0_y, grad_gx_s0_x, input, ), name = "grad_gx_0", dtype = hcl.Int(bits = 32))

    def grad_gx_1(grad_gx_s1_y, grad_gx_s1_x, input, grad_gx, grad_xx, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        grad_gx_s1_box__x = hcl.reduce_axis(0, 3)
        grad_gx_s1_box__y = hcl.reduce_axis(0, 3)
        return (grad_gx[grad_gx_s1_y, grad_gx_s1_x] + _sum(
            axis = [grad_gx_s1_box__y, grad_gx_s1_box__x, ],
            expr = grad_xx[(grad_gx_s1_box__y + grad_gx_s1_y), (grad_gx_s1_box__x + grad_gx_s1_x)]
        ))
    grad_gx_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gx_s1_y, grad_gx_s1_x, : grad_gx_1(grad_gx_s1_y, grad_gx_s1_x, input, grad_gx_0, grad_xx_0, ), name = "grad_gx_1", dtype = hcl.Int(bits = 32))

    def grad_y_0(grad_y_s0_y, grad_y_s0_x, input, padded16, ):
        return ((padded16[(grad_y_s0_y + 2), (grad_y_s0_x + 2)] + (((padded16[(grad_y_s0_y + 2), (grad_y_s0_x + 1)] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)) + (padded16[(grad_y_s0_y + 2), grad_y_s0_x] - padded16[grad_y_s0_y, grad_y_s0_x])) - (padded16[grad_y_s0_y, (grad_y_s0_x + 1)] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)))) - padded16[grad_y_s0_y, (grad_y_s0_x + 2)])
    grad_y_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_y_s0_y, grad_y_s0_x, : grad_y_0(grad_y_s0_y, grad_y_s0_x, input, padded16_0, ), name = "grad_y_0", dtype = hcl.Int(bits = 16))

    def grad_xy_0(grad_xy_s0_y, grad_xy_s0_x, input, grad_y, grad_x, ):
        return (hcl.cast(dtype = hcl.Int(bits = 32), expr = grad_x[grad_xy_s0_y, grad_xy_s0_x]) * hcl.cast(dtype = hcl.Int(bits = 32), expr = grad_y[grad_xy_s0_y, grad_xy_s0_x]))
    grad_xy_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_xy_s0_y, grad_xy_s0_x, : grad_xy_0(grad_xy_s0_y, grad_xy_s0_x, input, grad_y_0, grad_x_0, ), name = "grad_xy_0", dtype = hcl.Int(bits = 32))

    def grad_gxy_0(grad_gxy_s0_y, grad_gxy_s0_x, input, ):
        return 0
    grad_gxy_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gxy_s0_y, grad_gxy_s0_x, : grad_gxy_0(grad_gxy_s0_y, grad_gxy_s0_x, input, ), name = "grad_gxy_0", dtype = hcl.Int(bits = 32))

    def grad_gxy_1(grad_gxy_s1_y, grad_gxy_s1_x, input, grad_gxy, grad_xy, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        grad_gxy_s1_box__x = hcl.reduce_axis(0, 3)
        grad_gxy_s1_box__y = hcl.reduce_axis(0, 3)
        return (grad_gxy[grad_gxy_s1_y, grad_gxy_s1_x] + _sum(
            axis = [grad_gxy_s1_box__y, grad_gxy_s1_box__x, ],
            expr = grad_xy[(grad_gxy_s1_box__y + grad_gxy_s1_y), (grad_gxy_s1_box__x + grad_gxy_s1_x)]
        ))
    grad_gxy_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gxy_s1_y, grad_gxy_s1_x, : grad_gxy_1(grad_gxy_s1_y, grad_gxy_s1_x, input, grad_gxy_0, grad_xy_0, ), name = "grad_gxy_1", dtype = hcl.Int(bits = 32))

    def grad_yy_0(grad_yy_s0_y, grad_yy_s0_x, input, grad_y, ):
        t72_s = grad_y[grad_yy_s0_y, grad_yy_s0_x]
        return (hcl.cast(dtype = hcl.Int(bits = 32), expr = t72_s) * hcl.cast(dtype = hcl.Int(bits = 32), expr = t72_s))
    grad_yy_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_yy_s0_y, grad_yy_s0_x, : grad_yy_0(grad_yy_s0_y, grad_yy_s0_x, input, grad_y_0, ), name = "grad_yy_0", dtype = hcl.Int(bits = 32))

    def grad_gy_0(grad_gy_s0_y, grad_gy_s0_x, input, ):
        return 0
    grad_gy_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gy_s0_y, grad_gy_s0_x, : grad_gy_0(grad_gy_s0_y, grad_gy_s0_x, input, ), name = "grad_gy_0", dtype = hcl.Int(bits = 32))

    def grad_gy_1(grad_gy_s1_y, grad_gy_s1_x, input, grad_gy, grad_yy, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        grad_gy_s1_box__x = hcl.reduce_axis(0, 3)
        grad_gy_s1_box__y = hcl.reduce_axis(0, 3)
        return (grad_gy[grad_gy_s1_y, grad_gy_s1_x] + _sum(
            axis = [grad_gy_s1_box__y, grad_gy_s1_box__x, ],
            expr = grad_yy[(grad_gy_s1_box__y + grad_gy_s1_y), (grad_gy_s1_box__x + grad_gy_s1_x)]
        ))
    grad_gy_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gy_s1_y, grad_gy_s1_x, : grad_gy_1(grad_gy_s1_y, grad_gy_s1_x, input, grad_gy_0, grad_yy_0, ), name = "grad_gy_1", dtype = hcl.Int(bits = 32))

    def cim_0(cim_s0_y, cim_s0_x, input, grad_gy, grad_gxy, grad_gx, ):
        t73 = grad_gx[cim_s0_y, cim_s0_x]
        t74 = grad_gy[cim_s0_y, cim_s0_x]
        t75 = grad_gxy[cim_s0_y, cim_s0_x]
        t76 = (hcl.cast(dtype = hcl.Float(bits = 32), expr = (t73/144)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = (t74/144)))
        return (((hcl.cast(dtype = hcl.Float(bits = 32), expr = (t73/144)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = (t74/144))) - (hcl.cast(dtype = hcl.Float(bits = 32), expr = (t75/144)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = (t75/144)))) - ((t76 * t76) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.040000)))
    cim_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda cim_s0_y, cim_s0_x, : cim_0(cim_s0_y, cim_s0_x, input, grad_gy_1, grad_gxy_1, grad_gx_1, ), name = "cim_0", dtype = hcl.Float(bits = 32))

    def hw_output_0(hw_output_s0_y, hw_output_s0_x, input, cim, ):
        t77 = cim[(hw_output_s0_y + 1), (hw_output_s0_x + 1)]
        return hcl.select(hcl.and_((hcl.cast(dtype = hcl.Float(bits = 32), expr = 100.000000) <= t77), (hcl.select(cim[(hw_output_s0_y + 2), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)] > hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])))), cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)], hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))))), cim[(hw_output_s0_y + 2), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)] > hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])))), cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)], hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])))))) < t77)), hcl.cast(dtype = hcl.UInt(bits = 8), expr = 255), hcl.cast(dtype = hcl.UInt(bits = 8), expr = 0))
    hw_output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda hw_output_s0_y, hw_output_s0_x, : hw_output_0(hw_output_s0_y, hw_output_s0_x, input, cim_0, ), name = "hw_output_0", dtype = hcl.UInt(bits = 8))

    def output_0(output_s0_y, output_s0_x, input, hw_output, ):
        return hw_output[output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda output_s0_y, output_s0_x, : output_0(output_s0_y, output_s0_x, input, hw_output_0, ), name = "output_0", dtype = hcl.UInt(bits = 8))

    return output_0
input = hcl.placeholder((6000, 4000, ), name = "input", dtype = hcl.UInt(bits = 8))
s = hcl.create_schedule([input, ], _all)
s_output_0 = _all.output_0
s[s_output_0].unroll(s_output_0.axis[1], 2)
s[s_output_0].parallel(s_output_0.axis[0])
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 8))
output_shape = (5994, 3994)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
