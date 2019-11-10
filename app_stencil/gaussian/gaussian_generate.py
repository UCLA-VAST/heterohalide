import heterocl as hcl
hcl.init()
output_extent_0 = 2152
output_extent_1 = 3832
output_min_0 = 0
output_min_1 = 0
def _all(input, ):
    def sum_x_0(sum_x_s0_y, sum_x_s0_x, input, ):
        return hcl.cast(dtype = hcl.UInt(bits = 32), expr = 0)
    sum_x_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda sum_x_s0_y, sum_x_s0_x, : sum_x_0(sum_x_s0_y, sum_x_s0_x, input, ), name = "sum_x_0", dtype = hcl.UInt(bits = 32))

    def sum_x_1(sum_x_s1_y, sum_x_s1_x, input, sum_x, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        sum_x_s1_win2__x = hcl.reduce_axis(0, 9)
        sum_x_s1_win2__y = hcl.reduce_axis(0, 9)
        return (sum_x[sum_x_s1_y, sum_x_s1_x] + _sum(
            axis = [sum_x_s1_win2__y, sum_x_s1_win2__x, ],
            expr = ((hcl.cast(dtype = hcl.UInt(bits = 32), expr = input[(sum_x_s1_win2__y + sum_x_s1_y), (sum_x_s1_win2__x + sum_x_s1_x)]) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__x) * (sum_x_s1_win2__x + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794))))) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__y) * (sum_x_s1_win2__y + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794)))))
        ))
    sum_x_1 = hcl.compute((output_extent_1, output_extent_0, ), lambda sum_x_s1_y, sum_x_s1_x, : sum_x_1(sum_x_s1_y, sum_x_s1_x, input, sum_x_0, ), name = "sum_x_1", dtype = hcl.UInt(bits = 32))

    def blur_x_0(blur_x_s0_y, blur_x_s0_x, input, sum_x, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (sum_x[blur_x_s0_y, blur_x_s0_x]/hcl.cast(dtype = hcl.UInt(bits = 32), expr = 65536)))
    blur_x_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda blur_x_s0_y, blur_x_s0_x, : blur_x_0(blur_x_s0_y, blur_x_s0_x, input, sum_x_1, ), name = "blur_x_0", dtype = hcl.UInt(bits = 8))

    def hw_output_0(hw_output_s0_y, hw_output_s0_x, input, blur_x, ):
        return blur_x[hw_output_s0_y, hw_output_s0_x]
    hw_output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda hw_output_s0_y, hw_output_s0_x, : hw_output_0(hw_output_s0_y, hw_output_s0_x, input, blur_x_0, ), name = "hw_output_0", dtype = hcl.UInt(bits = 8))

    def output_0(output_s0_y, output_s0_x, input, hw_output, ):
        return hw_output[output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda output_s0_y, output_s0_x, : output_0(output_s0_y, output_s0_x, input, hw_output_0, ), name = "output_0", dtype = hcl.UInt(bits = 8))

    return output_0
input = hcl.placeholder((3840, 2160, ), name = "input", dtype = hcl.UInt(bits = 8))
s = hcl.create_schedule([input, ], _all)
s_blur_x_0 = _all.blur_x_0
s[s_blur_x_0].unroll(s_blur_x_0.axis[1], 8)
s_hw_output_0 = _all.hw_output_0
s[s_hw_output_0].unroll(s_hw_output_0.axis[1], 8)
s_output_0 = _all.output_0
s[s_output_0].unroll(s_output_0.axis[1], 8)
s_sum_x_0 = _all.sum_x_0
s[s_sum_x_0].unroll(s_sum_x_0.axis[1], 8)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 8))
output_shape = (3832, 2152)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
