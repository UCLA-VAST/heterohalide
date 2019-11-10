import heterocl as hcl
hcl.init()
output_extent_0 = 2442
output_extent_1 = 3258
output_min_0 = 0
output_min_1 = 0
def _all(input, ):
    def padded16_0(padded16_s0_y, padded16_s0_x, input, ):
        return hcl.cast(dtype = hcl.Int(bits = 16), expr = input[padded16_s0_y, padded16_s0_x])
    padded16_0 = hcl.compute(((output_extent_1 + 6), (output_extent_0 + 6), ), lambda padded16_s0_y, padded16_s0_x, : padded16_0(padded16_s0_y, padded16_s0_x, input, ), name = "padded16_0", dtype = hcl.Int(bits = 16))

    def grad_x_0(grad_x_s0_y, grad_x_s0_x, input, padded16, ):
        return (padded16[(grad_x_s0_y + 2), (grad_x_s0_x + 2)] + (((padded16[(grad_x_s0_y + 1), (grad_x_s0_x + 2)] * 2) + ((padded16[grad_x_s0_y, (grad_x_s0_x + 2)] - padded16[grad_x_s0_y, grad_x_s0_x]) - (padded16[(grad_x_s0_y + 1), grad_x_s0_x] * 2))) - padded16[(grad_x_s0_y + 2), grad_x_s0_x]))
    grad_x_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_x_s0_y, grad_x_s0_x, : grad_x_0(grad_x_s0_y, grad_x_s0_x, input, padded16_0, ), name = "grad_x_0", dtype = hcl.Int(bits = 16))

    def grad_xx_0(grad_xx_s0_y, grad_xx_s0_x, input, grad_x, ):
        t47_s = grad_x[grad_xx_s0_y, grad_xx_s0_x]
        return (hcl.cast(dtype = hcl.Int(bits = 32), expr = t47_s) * hcl.cast(dtype = hcl.Int(bits = 32), expr = t47_s))
    grad_xx_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_xx_s0_y, grad_xx_s0_x, : grad_xx_0(grad_xx_s0_y, grad_xx_s0_x, input, grad_x_0, ), name = "grad_xx_0", dtype = hcl.Int(bits = 32))

    def grad_gx_0(grad_gx_s0_y, grad_gx_s0_x, input, ):
        return 0
    grad_gx_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gx_s0_y, grad_gx_s0_x, : grad_gx_0(grad_gx_s0_y, grad_gx_s0_x, input, ), name = "grad_gx_0", dtype = hcl.Int(bits = 32))

    def grad_gx_1(grad_gx_s1_y, grad_gx_s1_x, input, grad_gx, grad_xx, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        grad_gx_s1_box__x = hcl.reduce_axis(0, 3)
        grad_gx_s1_box__y = hcl.reduce_axis(0, 3)
        return _sum(
            axis = [grad_gx_s1_box__y, grad_gx_s1_box__x, ],
            expr = grad_xx[(grad_gx_s1_box__y + grad_gx_s1_y), (grad_gx_s1_box__x + grad_gx_s1_x)]
        )
    grad_gx_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gx_s1_y, grad_gx_s1_x, : grad_gx_1(grad_gx_s1_y, grad_gx_s1_x, input, grad_gx_0, grad_xx_0, ), name = "grad_gx_1", dtype = hcl.Int(bits = 32))

    def grad_y_0(grad_y_s0_y, grad_y_s0_x, input, padded16, ):
        return ((padded16[(grad_y_s0_y + 2), (grad_y_s0_x + 2)] + (((padded16[(grad_y_s0_y + 2), (grad_y_s0_x + 1)] * 2) + (padded16[(grad_y_s0_y + 2), grad_y_s0_x] - padded16[grad_y_s0_y, grad_y_s0_x])) - (padded16[grad_y_s0_y, (grad_y_s0_x + 1)] * 2))) - padded16[grad_y_s0_y, (grad_y_s0_x + 2)])
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
        return _sum(
            axis = [grad_gxy_s1_box__y, grad_gxy_s1_box__x, ],
            expr = grad_xy[(grad_gxy_s1_box__y + grad_gxy_s1_y), (grad_gxy_s1_box__x + grad_gxy_s1_x)]
        )
    grad_gxy_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gxy_s1_y, grad_gxy_s1_x, : grad_gxy_1(grad_gxy_s1_y, grad_gxy_s1_x, input, grad_gxy_0, grad_xy_0, ), name = "grad_gxy_1", dtype = hcl.Int(bits = 32))

    def grad_yy_0(grad_yy_s0_y, grad_yy_s0_x, input, grad_y, ):
        t48_s = grad_y[grad_yy_s0_y, grad_yy_s0_x]
        return (hcl.cast(dtype = hcl.Int(bits = 32), expr = t48_s) * hcl.cast(dtype = hcl.Int(bits = 32), expr = t48_s))
    grad_yy_0 = hcl.compute(((output_extent_1 + 4), (output_extent_0 + 4), ), lambda grad_yy_s0_y, grad_yy_s0_x, : grad_yy_0(grad_yy_s0_y, grad_yy_s0_x, input, grad_y_0, ), name = "grad_yy_0", dtype = hcl.Int(bits = 32))

    def grad_gy_0(grad_gy_s0_y, grad_gy_s0_x, input, ):
        return 0
    grad_gy_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gy_s0_y, grad_gy_s0_x, : grad_gy_0(grad_gy_s0_y, grad_gy_s0_x, input, ), name = "grad_gy_0", dtype = hcl.Int(bits = 32))

    def grad_gy_1(grad_gy_s1_y, grad_gy_s1_x, input, grad_gy, grad_yy, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        grad_gy_s1_box__x = hcl.reduce_axis(0, 3)
        grad_gy_s1_box__y = hcl.reduce_axis(0, 3)
        return _sum(
            axis = [grad_gy_s1_box__y, grad_gy_s1_box__x, ],
            expr = grad_yy[(grad_gy_s1_box__y + grad_gy_s1_y), (grad_gy_s1_box__x + grad_gy_s1_x)]
        )
    grad_gy_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda grad_gy_s1_y, grad_gy_s1_x, : grad_gy_1(grad_gy_s1_y, grad_gy_s1_x, input, grad_gy_0, grad_yy_0, ), name = "grad_gy_1", dtype = hcl.Int(bits = 32))

    def cim_0(cim_s0_y, cim_s0_x, input, grad_gy, grad_gxy, grad_gx, ):
        t49 = hcl.cast(dtype = hcl.Float(bits = 32), expr = grad_gx[cim_s0_y, cim_s0_x])
        t50 = hcl.cast(dtype = hcl.Float(bits = 32), expr = grad_gy[cim_s0_y, cim_s0_x])
        t51 = hcl.cast(dtype = hcl.Float(bits = 32), expr = grad_gxy[cim_s0_y, cim_s0_x])
        t52 = (hcl.cast(dtype = hcl.Float(bits = 32), expr = (t49/144)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = (t50/144)))
        return (((hcl.cast(dtype = hcl.Float(bits = 32), expr = (t49/144)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = (t50/144))) - (hcl.cast(dtype = hcl.Float(bits = 32), expr = (t51/144)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = (t51/144)))) - ((t52 * t52) * float(0.040000)))
    cim_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda cim_s0_y, cim_s0_x, : cim_0(cim_s0_y, cim_s0_x, input, grad_gy_1, grad_gxy_1, grad_gx_1, ), name = "cim_0", dtype = hcl.Float(bits = 32))

    def hw_output_0(hw_output_s0_y, hw_output_s0_x, input, cim, ):
        t53 = cim[(hw_output_s0_y + 1), (hw_output_s0_x + 1)]
        return hcl.select(hcl.and_((float(100.000000) <= t53), (hcl.select(cim[(hw_output_s0_y + 2), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)] > hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])))), cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)], hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))))), cim[(hw_output_s0_y + 2), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)] > hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])))), cim[(hw_output_s0_y + 2), (hw_output_s0_x + 1)], hcl.select(cim[(hw_output_s0_y + 2), hw_output_s0_x] > hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]))), cim[(hw_output_s0_y + 2), hw_output_s0_x], hcl.select(cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)] > hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])), cim[(hw_output_s0_y + 1), (hw_output_s0_x + 2)], hcl.select(cim[(hw_output_s0_y + 1), hw_output_s0_x] > hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)]), cim[(hw_output_s0_y + 1), hw_output_s0_x], hcl.select(cim[hw_output_s0_y, hw_output_s0_x] > cim[hw_output_s0_y, (hw_output_s0_x + 1)], cim[hw_output_s0_y, hw_output_s0_x], cim[hw_output_s0_y, (hw_output_s0_x + 1)])))))) < t53)), 255, 0)
    hw_output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda hw_output_s0_y, hw_output_s0_x, : hw_output_0(hw_output_s0_y, hw_output_s0_x, input, cim_0, ), name = "hw_output_0", dtype = hcl.UInt(bits = 16))

    def output_0(output_s0_y, output_s0_x, input, hw_output, ):
        return hw_output[output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda output_s0_y, output_s0_x, : output_0(output_s0_y, output_s0_x, input, hw_output_0, ), name = "output_0", dtype = hcl.UInt(bits = 16))

    return output_0
input = hcl.placeholder((3264, 2448, ), name = "input")
s = hcl.create_schedule([input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (1, 0))
hcl_input = hcl.asarray(np_input)
output_shape = (3258, 2442)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl_optimize_remain_hw_output.npy", np_out)
print(hcl.build(s, target = "soda"))