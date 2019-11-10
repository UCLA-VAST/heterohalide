import heterocl as hcl
hcl.init()
output_extent_0 = 766
output_extent_1 = 1278
output_extent_2 = 3
output_min_0 = 0
output_min_1 = 0
output_min_2 = 0
def _all(input, ):
    def linear_0(linear_s0_c, linear_s0_y, linear_s0_x, input, ):
        t8 = input[linear_s0_c, linear_s0_y, linear_s0_x]
        return hcl.select((hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.040450) < t8), hcl.power(((t8 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.947867)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.052133)), hcl.cast(dtype = hcl.Float(bits = 32), expr = 2.400000)), (t8 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.077399)))
    linear_0 = hcl.compute((output_extent_2, (output_extent_1 + 2), (output_extent_0 + 2), ), lambda linear_s0_c, linear_s0_y, linear_s0_x, : linear_0(linear_s0_c, linear_s0_y, linear_s0_x, input, ), name = "linear_0", dtype = hcl.Float(bits = 32))

    def blur_x_0(blur_x_s0_c, blur_x_s0_y, blur_x_s0_x, input, linear, ):
        return ((linear[blur_x_s0_c, blur_x_s0_y, (blur_x_s0_x + 2)] + (linear[blur_x_s0_c, blur_x_s0_y, blur_x_s0_x] + linear[blur_x_s0_c, blur_x_s0_y, (blur_x_s0_x + 1)])) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.333333))
    blur_x_0 = hcl.compute((output_extent_2, (output_extent_1 + 2), output_extent_0, ), lambda blur_x_s0_c, blur_x_s0_y, blur_x_s0_x, : blur_x_0(blur_x_s0_c, blur_x_s0_y, blur_x_s0_x, input, linear_0, ), name = "blur_x_0", dtype = hcl.Float(bits = 32))

    def blur_y_0(blur_y_s0_c, blur_y_s0_y, blur_y_s0_x, input, blur_x, ):
        return ((blur_x[blur_y_s0_c, (blur_y_s0_y + 2), blur_y_s0_x] + (blur_x[blur_y_s0_c, blur_y_s0_y, blur_y_s0_x] + blur_x[blur_y_s0_c, (blur_y_s0_y + 1), blur_y_s0_x])) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.333333))
    blur_y_0 = hcl.compute((output_extent_2, output_extent_1, output_extent_0, ), lambda blur_y_s0_c, blur_y_s0_y, blur_y_s0_x, : blur_y_0(blur_y_s0_c, blur_y_s0_y, blur_y_s0_x, input, blur_x_0, ), name = "blur_y_0", dtype = hcl.Float(bits = 32))

    def srgb_0(srgb_s0_c, srgb_s0_y, srgb_s0_x, input, blur_y, ):
        t9 = blur_y[srgb_s0_c, srgb_s0_y, srgb_s0_x]
        return hcl.select((hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.003131) < t9), ((hcl.power(t9, hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.416667)) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 1.055000)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = -0.055000)), (t9 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 12.920000)))
    srgb_0 = hcl.compute((output_extent_2, output_extent_1, output_extent_0, ), lambda srgb_s0_c, srgb_s0_y, srgb_s0_x, : srgb_0(srgb_s0_c, srgb_s0_y, srgb_s0_x, input, blur_y_0, ), name = "srgb_0", dtype = hcl.Float(bits = 32))

    def output_0(output_s0_c, output_s0_y, output_s0_x, input, srgb, ):
        return srgb[output_s0_c, output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_2, output_extent_1, output_extent_0, ), lambda output_s0_c, output_s0_y, output_s0_x, : output_0(output_s0_c, output_s0_y, output_s0_x, input, srgb_0, ), name = "output_0", dtype = hcl.Float(bits = 32))

    return output_0
input = hcl.placeholder((3, 1280, 768, ), name = "input", dtype = hcl.Float(bits = 32))
s = hcl.create_schedule([input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (2, 1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.Float(bits = 32))
output_shape = (3, 1278, 766)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (2, 1, 0))
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
