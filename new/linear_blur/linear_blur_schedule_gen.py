import heterocl as hcl
hcl.init()
final_extent_0 = 766
final_extent_1 = 1278
final_extent_2 = 3
final_min_0 = 0
final_min_1 = 0
final_min_2 = 0
def top(input, ):
    linear = hcl.compute(((final_extent_0 + 2), (final_extent_1 + 2), final_extent_2), lambda x, y, z: 0, name = "linear", dtype = hcl.Float(bits = 32))
    with hcl.Stage("linear"):
        with hcl.for_(final_min_2, final_extent_2, name = "linear_s0_c") as linear_s0_c:
            with hcl.for_(final_min_1, (final_extent_1 + 2), name = "linear_s0_y") as linear_s0_y:
                with hcl.for_(final_min_0, (final_extent_0 + 2), name = "linear_s0_x") as linear_s0_x:
                    t8 = input[linear_s0_x, linear_s0_y, linear_s0_c]
                    linear[linear_s0_x, linear_s0_y, linear_s0_c] = hcl.select((hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.040450) < t8), hcl.power(((t8 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.947867)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.052133)), hcl.cast(dtype = hcl.Float(bits = 32), expr = 2.400000)), (t8 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.077399)))
    blur_x = hcl.compute((final_extent_0, (final_extent_1 + 2), final_extent_2), lambda x, y, z: 0, name = "blur_x", dtype = hcl.Float(bits = 32))
    with hcl.Stage("blur_x"):
        with hcl.for_(final_min_2, final_extent_2, name = "blur_x_s0_c") as blur_x_s0_c:
            with hcl.for_(final_min_0, final_extent_0, name = "blur_x_s0_x") as blur_x_s0_x:
                with hcl.for_(final_min_1, (final_extent_1 + 2), name = "blur_x_s0_y") as blur_x_s0_y:
                    blur_x[blur_x_s0_x, blur_x_s0_y, blur_x_s0_c] = ((linear[(blur_x_s0_x + 2), blur_x_s0_y, blur_x_s0_c] + (linear[blur_x_s0_x, blur_x_s0_y, blur_x_s0_c] + linear[(blur_x_s0_x + 1), blur_x_s0_y, blur_x_s0_c])) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.333333))
    blur_y = hcl.compute((final_extent_0, final_extent_1, final_extent_2), lambda x, y, z: 0, name = "blur_y", dtype = hcl.Float(bits = 32))
    with hcl.Stage("blur_y"):
        with hcl.for_(final_min_2, final_extent_2, name = "blur_y_s0_c") as blur_y_s0_c:
            with hcl.for_(final_min_1, final_extent_1, name = "blur_y_s0_y") as blur_y_s0_y:
                with hcl.for_(final_min_0, final_extent_0, name = "blur_y_s0_x") as blur_y_s0_x:
                    blur_y[blur_y_s0_x, blur_y_s0_y, blur_y_s0_c] = ((blur_x[blur_y_s0_x, (blur_y_s0_y + 2), blur_y_s0_c] + (blur_x[blur_y_s0_x, blur_y_s0_y, blur_y_s0_c] + blur_x[blur_y_s0_x, (blur_y_s0_y + 1), blur_y_s0_c])) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.333333))
    srgb = hcl.compute((final_extent_0, final_extent_1, final_extent_2), lambda x, y, z: 0, name = "srgb", dtype = hcl.Float(bits = 32))
    with hcl.Stage("srgb"):
        with hcl.for_(final_min_2, final_extent_2, name = "srgb_s0_c") as srgb_s0_c:
            with hcl.for_(final_min_1, final_extent_1, name = "srgb_s0_y") as srgb_s0_y:
                with hcl.for_(final_min_0, final_extent_0, name = "srgb_s0_x") as srgb_s0_x:
                    t9 = blur_y[srgb_s0_x, srgb_s0_y, srgb_s0_c]
                    srgb[srgb_s0_x, srgb_s0_y, srgb_s0_c] = hcl.select((hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.003131) < t9), ((hcl.power(t9, hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.416667)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 1.055000)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = -0.055000)), (t9 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 12.920000)))
    final = hcl.compute((766, 1278, 3), lambda x, y, z: 0, name = "final", dtype = hcl.Float(bits = 32))
    with hcl.Stage("final"):
        with hcl.for_(final_min_2, final_extent_2, name = "final_s0_c") as final_s0_c:
            with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
                with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                    final[final_s0_x, final_s0_y, final_s0_c] = srgb[final_s0_x, final_s0_y, final_s0_c]
    return final
input = hcl.placeholder((768, 1280, 3, ), name = "input", dtype = hcl.Float(bits = 32))
s = hcl.create_schedule([input, ], top)
s_blur_x = top.blur_x
s[s_blur_x].unroll(s_blur_x.axis[1], 4)
s_blur_y = top.blur_y
s[s_blur_y].unroll(s_blur_y.axis[2], 4)
s_linear = top.linear
s[s_linear].unroll(s_linear.axis[1], 16)
s_blur_x = top.blur_x
s[s_blur_x].parallel(s_blur_x.axis[2])
s_blur_y = top.blur_y
s[s_blur_y].parallel(s_blur_y.axis[1])
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.Float(bits = 32))
output_shape = (766, 1278, 3, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
