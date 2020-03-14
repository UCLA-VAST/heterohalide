import heterocl as hcl
hcl.init()
final_extent_0 = 64
final_extent_1 = 64
final_extent_2 = 32
final_extent_3 = 4
final_min_0 = 0
final_min_1 = 0
final_min_2 = 0
final_min_3 = 0
def top(input, filter, bias, ):
    input_extent_3_required_s = (((((final_extent_2 + 31)//32) * final_extent_3) + -1)//hcl.select(((final_extent_2 + 31)//32) > 1, ((final_extent_2 + 31)//32), 1))
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    final_total_extent_2 = (final_total_extent_1 * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_2))
    final_total_extent_3 = (final_total_extent_2 * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_3))
    f_conv_n_extent_realized_s = hcl.select(hcl.select((((final_extent_2 * final_extent_3) + -1)//hcl.select(final_extent_2 > 1, final_extent_2, 1)) > (final_extent_3 + -1), (((final_extent_2 * final_extent_3) + -1)//hcl.select(final_extent_2 > 1, final_extent_2, 1)), (final_extent_3 + -1)) > (((((final_extent_2 + 31)//32) * final_extent_3) + -1)//(hcl.select(((final_extent_2 + -1)//32) > 0, ((final_extent_2 + -1)//32), 0) + 1)), hcl.select((((final_extent_2 * final_extent_3) + -1)//hcl.select(final_extent_2 > 1, final_extent_2, 1)) > (final_extent_3 + -1), (((final_extent_2 * final_extent_3) + -1)//hcl.select(final_extent_2 > 1, final_extent_2, 1)), (final_extent_3 + -1)), (((((final_extent_2 + 31)//32) * final_extent_3) + -1)//(hcl.select(((final_extent_2 + -1)//32) > 0, ((final_extent_2 + -1)//32), 0) + 1)))
    f_conv_z_extent_realized = hcl.select(((hcl.select(((final_extent_2 + -1)//32) > 0, ((final_extent_2 + -1)//32), 0) * 32) + 32) > final_extent_2, ((hcl.select(((final_extent_2 + -1)//32) > 0, ((final_extent_2 + -1)//32), 0) * 32) + 32), final_extent_2)
    f_conv = hcl.compute((final_extent_0, ((((final_extent_1 + -1)//32) * 32) + 32), f_conv_z_extent_realized, (f_conv_n_extent_realized_s + 1)), lambda x, y, z, w: 0, name = "f_conv", dtype = hcl.Float(bits = 32))
    with hcl.Stage("f_conv"):
        with hcl.for_(0, (final_extent_2 * final_extent_3), name = "f_conv_s0_z_par") as f_conv_s0_z_par:
            with hcl.for_(final_min_1, final_extent_1, name = "f_conv_s0_y") as f_conv_s0_y:
                with hcl.for_(final_min_0, final_extent_0, name = "f_conv_s0_x") as f_conv_s0_x:
                    f_conv[f_conv_s0_x, f_conv_s0_y, ((f_conv_s0_z_par % hcl.select(final_extent_2 > 1, final_extent_2, 1)) + final_min_2), ((f_conv_s0_z_par//hcl.select(final_extent_2 > 1, final_extent_2, 1)) + final_min_3)] = bias[((f_conv_s0_z_par % hcl.select(final_extent_2 > 1, final_extent_2, 1)) + final_min_2)]
        with hcl.for_(0, (((final_extent_2 + 31)//32) * final_extent_3), name = "f_conv_s1_z_z_par") as f_conv_s1_z_z_par:
            f_conv_s1_z_z_t_base_s = (f_conv_s1_z_z_par % hcl.select(((final_extent_2 + 31)//32) > 1, ((final_extent_2 + 31)//32), 1))
            with hcl.for_(0, 32, name = "f_conv_s1_r__z") as f_conv_s1_r__z:
                with hcl.for_(0, ((final_extent_1 + 31)//32), name = "f_conv_s1_y_y") as f_conv_s1_y_y:
                    with hcl.for_(0, 32, name = "f_conv_s1_z_z_t") as f_conv_s1_z_z_t:
                        with hcl.for_(0, 32, name = "f_conv_s1_y_y_t") as f_conv_s1_y_y_t:
                            with hcl.for_(final_min_0, final_extent_0, name = "f_conv_s1_x") as f_conv_s1_x:
                                with hcl.for_(0, 3, name = "f_conv_s1_r__y_r21") as f_conv_s1_r__y_r21:
                                    with hcl.for_(0, 3, name = "f_conv_s1_r__x_r20") as f_conv_s1_r__x_r20:
                                        t51_s = (f_conv_s1_z_z_par//hcl.select(((final_extent_2 + 31)//32) > 1, ((final_extent_2 + 31)//32), 1))
                                        f_conv[f_conv_s1_x, (((f_conv_s1_y_y * 32) + final_min_1) + f_conv_s1_y_y_t), (((f_conv_s1_z_z_t_base_s * 32) + final_min_2) + f_conv_s1_z_z_t), ((f_conv_s1_z_z_par//hcl.select(((final_extent_2 + 31)//32) > 1, ((final_extent_2 + 31)//32), 1)) + final_min_3)] = (f_conv[f_conv_s1_x, (((f_conv_s1_y_y * 32) + final_min_1) + f_conv_s1_y_y_t), (((f_conv_s1_z_z_t_base_s * 32) + final_min_2) + f_conv_s1_z_z_t), (final_min_3 + t51_s)] + (filter[f_conv_s1_r__x_r20, f_conv_s1_r__y_r21, f_conv_s1_r__z, (((f_conv_s1_z_z_t_base_s * 32) + final_min_2) + f_conv_s1_z_z_t)] * input[(f_conv_s1_r__x_r20 + f_conv_s1_x), ((((f_conv_s1_y_y * 32) + final_min_1) + f_conv_s1_y_y_t) + f_conv_s1_r__y_r21), f_conv_s1_r__z, (final_min_3 + t51_s)]))
    final = hcl.compute((64, 64, 32, 4), lambda x, y, z, w: 0, name = "final", dtype = hcl.Float(bits = 32))
    with hcl.Stage("final"):
        with hcl.for_(final_min_3, final_extent_3, name = "final_s0_n") as final_s0_n:
            with hcl.for_(final_min_2, final_extent_2, name = "final_s0_z") as final_s0_z:
                with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
                    with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                        final[final_s0_x, final_s0_y, final_s0_z, final_s0_n] = hcl.select(f_conv[final_s0_x, final_s0_y, final_s0_z, final_s0_n] > hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000), f_conv[final_s0_x, final_s0_y, final_s0_z, final_s0_n], hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000))
    return final
input = hcl.placeholder((67, 67, 32, 4, ), name = "input", dtype = hcl.Float(bits = 32))
filter = hcl.placeholder((3, 3, 32, 32, ), name = "filter", dtype = hcl.Float(bits = 32))
bias = hcl.placeholder((32, ), name = "bias", dtype = hcl.Float(bits = 32))
s = hcl.create_schedule([input, filter, bias, ], top)
s_f_conv = top.f_conv
s[s_f_conv].unroll(s_f_conv.axis[6], 3)
s[s_f_conv].unroll(s_f_conv.axis[7], 3)
s_f_conv = top.f_conv
s[s_f_conv].parallel(s_f_conv.axis[0])
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.Float(bits = 32))
np_filter = np.load("filter.npy")
hcl_filter = hcl.asarray(np_filter, dtype = hcl.Float(bits = 32))
np_bias = np.load("bias.npy")
hcl_bias = hcl.asarray(np_bias, dtype = hcl.Float(bits = 32))
output_shape = (64, 64, 32, 4, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
f(hcl_input, hcl_filter, hcl_bias, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "merlinc"))
