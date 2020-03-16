import heterocl as hcl
hcl.init()
final_extent_0 = 2442
final_extent_1 = 3258
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    padded16 = hcl.compute(((final_extent_0 + 6), (final_extent_1 + 6)), lambda x, y: 0, name = "padded16", dtype = hcl.Int(bits = 16))
    with hcl.Stage("padded16"):
        with hcl.for_(final_min_1, (final_extent_1 + 6), name = "padded16_s0_y") as padded16_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 6), name = "padded16_s0_x") as padded16_s0_x:
                padded16[padded16_s0_x, padded16_s0_y] = hcl.cast(dtype = hcl.Int(bits = 16), expr = input[padded16_s0_x, padded16_s0_y])
    grad_x = hcl.compute(((final_extent_0 + 4), (final_extent_1 + 4)), lambda x, y: 0, name = "grad_x", dtype = hcl.Int(bits = 16))
    with hcl.Stage("grad_x"):
        with hcl.for_(final_min_1, (final_extent_1 + 4), name = "grad_x_s0_y") as grad_x_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 4), name = "grad_x_s0_x") as grad_x_s0_x:
                grad_x[grad_x_s0_x, grad_x_s0_y] = (padded16[(grad_x_s0_x + 2), (grad_x_s0_y + 2)] + (((padded16[(grad_x_s0_x + 2), (grad_x_s0_y + 1)] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)) + ((padded16[(grad_x_s0_x + 2), grad_x_s0_y] - padded16[grad_x_s0_x, grad_x_s0_y]) - (padded16[grad_x_s0_x, (grad_x_s0_y + 1)] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)))) - padded16[grad_x_s0_x, (grad_x_s0_y + 2)]))
    grad_xx = hcl.compute(((final_extent_0 + 4), (final_extent_1 + 4)), lambda x, y: 0, name = "grad_xx", dtype = hcl.Int(bits = 32))
    with hcl.Stage("grad_xx"):
        with hcl.for_(final_min_1, (final_extent_1 + 4), name = "grad_xx_s0_y") as grad_xx_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 4), name = "grad_xx_s0_x") as grad_xx_s0_x:
                t30_s = grad_x[grad_xx_s0_x, grad_xx_s0_y]
                grad_xx[grad_xx_s0_x, grad_xx_s0_y] = (hcl.cast(dtype = hcl.Int(bits = 32), expr = t30_s) * hcl.cast(dtype = hcl.Int(bits = 32), expr = t30_s))
    grad_gx = hcl.compute(((final_extent_0 + 2), (final_extent_1 + 2)), lambda x, y: 0, name = "grad_gx", dtype = hcl.Int(bits = 32))
    with hcl.Stage("grad_gx"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "grad_gx_s0_y") as grad_gx_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "grad_gx_s0_x") as grad_gx_s0_x:
                grad_gx[grad_gx_s0_x, grad_gx_s0_y] = 0
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "grad_gx_s1_y") as grad_gx_s1_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "grad_gx_s1_x") as grad_gx_s1_x:
                with hcl.for_(0, 3, name = "grad_gx_s1_box__y") as grad_gx_s1_box__y:
                    with hcl.for_(0, 3, name = "grad_gx_s1_box__x") as grad_gx_s1_box__x:
                        grad_gx[grad_gx_s1_x, grad_gx_s1_y] = (grad_gx[grad_gx_s1_x, grad_gx_s1_y] + grad_xx[(grad_gx_s1_box__x + grad_gx_s1_x), (grad_gx_s1_box__y + grad_gx_s1_y)])
    grad_y = hcl.compute(((final_extent_0 + 4), (final_extent_1 + 4)), lambda x, y: 0, name = "grad_y", dtype = hcl.Int(bits = 16))
    with hcl.Stage("grad_y"):
        with hcl.for_(final_min_1, (final_extent_1 + 4), name = "grad_y_s0_y") as grad_y_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 4), name = "grad_y_s0_x") as grad_y_s0_x:
                grad_y[grad_y_s0_x, grad_y_s0_y] = ((padded16[(grad_y_s0_x + 2), (grad_y_s0_y + 2)] + (((padded16[(grad_y_s0_x + 1), (grad_y_s0_y + 2)] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)) + (padded16[grad_y_s0_x, (grad_y_s0_y + 2)] - padded16[grad_y_s0_x, grad_y_s0_y])) - (padded16[(grad_y_s0_x + 1), grad_y_s0_y] * hcl.cast(dtype = hcl.Int(bits = 16), expr = 2)))) - padded16[(grad_y_s0_x + 2), grad_y_s0_y])
    grad_xy = hcl.compute(((final_extent_0 + 4), (final_extent_1 + 4)), lambda x, y: 0, name = "grad_xy", dtype = hcl.Int(bits = 32))
    with hcl.Stage("grad_xy"):
        with hcl.for_(final_min_1, (final_extent_1 + 4), name = "grad_xy_s0_y") as grad_xy_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 4), name = "grad_xy_s0_x") as grad_xy_s0_x:
                grad_xy[grad_xy_s0_x, grad_xy_s0_y] = (hcl.cast(dtype = hcl.Int(bits = 32), expr = grad_x[grad_xy_s0_x, grad_xy_s0_y]) * hcl.cast(dtype = hcl.Int(bits = 32), expr = grad_y[grad_xy_s0_x, grad_xy_s0_y]))
    grad_gxy = hcl.compute(((final_extent_0 + 2), (final_extent_1 + 2)), lambda x, y: 0, name = "grad_gxy", dtype = hcl.Int(bits = 32))
    with hcl.Stage("grad_gxy"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "grad_gxy_s0_y") as grad_gxy_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "grad_gxy_s0_x") as grad_gxy_s0_x:
                grad_gxy[grad_gxy_s0_x, grad_gxy_s0_y] = 0
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "grad_gxy_s1_y") as grad_gxy_s1_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "grad_gxy_s1_x") as grad_gxy_s1_x:
                with hcl.for_(0, 3, name = "grad_gxy_s1_box__y") as grad_gxy_s1_box__y:
                    with hcl.for_(0, 3, name = "grad_gxy_s1_box__x") as grad_gxy_s1_box__x:
                        grad_gxy[grad_gxy_s1_x, grad_gxy_s1_y] = (grad_gxy[grad_gxy_s1_x, grad_gxy_s1_y] + grad_xy[(grad_gxy_s1_box__x + grad_gxy_s1_x), (grad_gxy_s1_box__y + grad_gxy_s1_y)])
    grad_yy = hcl.compute(((final_extent_0 + 4), (final_extent_1 + 4)), lambda x, y: 0, name = "grad_yy", dtype = hcl.Int(bits = 32))
    with hcl.Stage("grad_yy"):
        with hcl.for_(final_min_1, (final_extent_1 + 4), name = "grad_yy_s0_y") as grad_yy_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 4), name = "grad_yy_s0_x") as grad_yy_s0_x:
                t31_s = grad_y[grad_yy_s0_x, grad_yy_s0_y]
                grad_yy[grad_yy_s0_x, grad_yy_s0_y] = (hcl.cast(dtype = hcl.Int(bits = 32), expr = t31_s) * hcl.cast(dtype = hcl.Int(bits = 32), expr = t31_s))
    grad_gy = hcl.compute(((final_extent_0 + 2), (final_extent_1 + 2)), lambda x, y: 0, name = "grad_gy", dtype = hcl.Int(bits = 32))
    with hcl.Stage("grad_gy"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "grad_gy_s0_y") as grad_gy_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "grad_gy_s0_x") as grad_gy_s0_x:
                grad_gy[grad_gy_s0_x, grad_gy_s0_y] = 0
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "grad_gy_s1_y") as grad_gy_s1_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "grad_gy_s1_x") as grad_gy_s1_x:
                with hcl.for_(0, 3, name = "grad_gy_s1_box__y") as grad_gy_s1_box__y:
                    with hcl.for_(0, 3, name = "grad_gy_s1_box__x") as grad_gy_s1_box__x:
                        grad_gy[grad_gy_s1_x, grad_gy_s1_y] = (grad_gy[grad_gy_s1_x, grad_gy_s1_y] + grad_yy[(grad_gy_s1_box__x + grad_gy_s1_x), (grad_gy_s1_box__y + grad_gy_s1_y)])
    cim = hcl.compute(((final_extent_0 + 2), (final_extent_1 + 2)), lambda x, y: 0, name = "cim", dtype = hcl.Float(bits = 32))
    with hcl.Stage("cim"):
        with hcl.for_(final_min_1, (final_extent_1 + 2), name = "cim_s0_y") as cim_s0_y:
            with hcl.for_(final_min_0, (final_extent_0 + 2), name = "cim_s0_x") as cim_s0_x:
                t32 = grad_gx[cim_s0_x, cim_s0_y]
                t33 = grad_gy[cim_s0_x, cim_s0_y]
                t34 = grad_gxy[cim_s0_x, cim_s0_y]
                t35 = (hcl.cast(dtype = hcl.Float(bits = 32), expr = (t32//144)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = (t33//144)))
                cim[cim_s0_x, cim_s0_y] = (((hcl.cast(dtype = hcl.Float(bits = 32), expr = (t32//144)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = (t33//144))) - (hcl.cast(dtype = hcl.Float(bits = 32), expr = (t34//144)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = (t34//144)))) - ((t35 * t35) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.040000)))
    output_final = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "output_final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("output_final"):
        with hcl.for_(final_min_1, final_extent_1, name = "output_final_s0_y") as output_final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "output_final_s0_x") as output_final_s0_x:
                t36 = cim[(output_final_s0_x + 1), (output_final_s0_y + 1)]
                output_final[output_final_s0_x, output_final_s0_y] = hcl.select(hcl.and_((hcl.cast(dtype = hcl.Float(bits = 32), expr = 100.000000) <= t36), (hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 2)] > hcl.select(cim[(output_final_s0_x + 1), (output_final_s0_y + 2)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 2)] > hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]))), cim[output_final_s0_x, (output_final_s0_y + 2)], hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])))), cim[(output_final_s0_x + 1), (output_final_s0_y + 2)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 2)] > hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]))), cim[output_final_s0_x, (output_final_s0_y + 2)], hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]))))), cim[(output_final_s0_x + 2), (output_final_s0_y + 2)], hcl.select(cim[(output_final_s0_x + 1), (output_final_s0_y + 2)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 2)] > hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]))), cim[output_final_s0_x, (output_final_s0_y + 2)], hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])))), cim[(output_final_s0_x + 1), (output_final_s0_y + 2)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 2)] > hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]))), cim[output_final_s0_x, (output_final_s0_y + 2)], hcl.select(cim[(output_final_s0_x + 2), (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])), cim[(output_final_s0_x + 2), (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, (output_final_s0_y + 1)] > hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y]), cim[output_final_s0_x, (output_final_s0_y + 1)], hcl.select(cim[output_final_s0_x, output_final_s0_y] > cim[(output_final_s0_x + 1), output_final_s0_y], cim[output_final_s0_x, output_final_s0_y], cim[(output_final_s0_x + 1), output_final_s0_y])))))) < t36)), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 0))
    final = hcl.compute((2442, 3258), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = output_final[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((2448, 3264, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (2442, 3258, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
print(hcl.build(s, target = "merlinc"))