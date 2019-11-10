import heterocl as hcl
hcl.init()
blur_y_extent_0 = 640
blur_y_extent_1 = 480
blur_y_min_0 = 0
blur_y_min_1 = 0
def _all(input, ):
    def blur_x_0(blur_x_s0_y, blur_x_s0_x, input, ):
        return ((input[blur_x_s0_y, (blur_x_s0_x + hcl.cast(dtype = hcl.Int(bits = 32), expr = 2))] + (input[blur_x_s0_y, blur_x_s0_x] + input[blur_x_s0_y, (blur_x_s0_x + hcl.cast(dtype = hcl.Int(bits = 32), expr = 1))]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3))
    blur_x_0 = hcl.compute(((blur_y_extent_1 + hcl.cast(dtype = hcl.Int(bits = 32), expr = 2)), blur_y_extent_0, ), lambda blur_x_s0_y, blur_x_s0_x, : blur_x_0(blur_x_s0_y, blur_x_s0_x, input, ), name = "blur_x_0", dtype = hcl.UInt(bits = 16))

    def blur_y_0(blur_y_s0_y, blur_y_s0_x, input, blur_x, ):
        return ((blur_x[(blur_y_s0_y + hcl.cast(dtype = hcl.Int(bits = 32), expr = 2)), blur_y_s0_x] + (blur_x[blur_y_s0_y, blur_y_s0_x] + blur_x[(blur_y_s0_y + hcl.cast(dtype = hcl.Int(bits = 32), expr = 1)), blur_y_s0_x]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 3))
    blur_y_0 = hcl.compute((blur_y_extent_1, blur_y_extent_0, ), lambda blur_y_s0_y, blur_y_s0_x, : blur_y_0(blur_y_s0_y, blur_y_s0_x, input, blur_x_0, ), name = "blur_y_0", dtype = hcl.UInt(bits = 16))

    return blur_y_0
input = hcl.placeholder((482, 648, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], _all)
s_blur_x_0 = _all.blur_x_0
s[s_blur_x_0].unroll(s_blur_x_0.axis[1], 4)
s_blur_y_0 = _all.blur_y_0
s[s_blur_y_0].unroll(s_blur_y_0.axis[1], 4)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("/curr/jiajieli/app_halide2heterocl/blur/input.npy"), (1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (480, 640)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
