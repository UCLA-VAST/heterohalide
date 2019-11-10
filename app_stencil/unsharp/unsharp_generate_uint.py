import heterocl as hcl
hcl.init()
output_extent_0 = 8
output_extent_1 = 8
output_extent_2 = 3
output_min_0 = 0
output_min_1 = 0
output_min_2 = 0
def _all(input, ):
    def gray_0(gray_s0_y, gray_s0_x, input, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (((input[hcl.cast(dtype = hcl.Int(bits = 32), expr = 2), gray_s0_y, gray_s0_x] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 29)) + ((input[hcl.cast(dtype = hcl.Int(bits = 32), expr = 0), gray_s0_y, gray_s0_x] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 77)) + (input[hcl.cast(dtype = hcl.Int(bits = 32), expr = 1), gray_s0_y, gray_s0_x] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 150))))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 256)))
    gray_0 = hcl.compute(((output_extent_1 + hcl.cast(dtype = hcl.Int(bits = 32), expr = 8)), (output_extent_0 + hcl.cast(dtype = hcl.Int(bits = 32), expr = 8)), ), lambda gray_s0_y, gray_s0_x, : gray_0(gray_s0_y, gray_s0_x, input, ), name = "gray_0", dtype = hcl.UInt(bits = 8))

    def sum_x_0(sum_x_s0_y, sum_x_s0_x, input, ):
        return hcl.cast(dtype = hcl.UInt(bits = 32), expr = 0)
    sum_x_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda sum_x_s0_y, sum_x_s0_x, : sum_x_0(sum_x_s0_y, sum_x_s0_x, input, ), name = "sum_x_0", dtype = hcl.UInt(bits = 32))

    def sum_x_1(sum_x_s1_y, sum_x_s1_x, input, sum_x, gray, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        sum_x_s1_win2__x = hcl.reduce_axis(0, 9)
        sum_x_s1_win2__y = hcl.reduce_axis(0, 9)
        return (sum_x[sum_x_s1_y, sum_x_s1_x] + _sum(
            axis = [sum_x_s1_win2__y, sum_x_s1_win2__x, ],
            expr = ((hcl.cast(dtype = hcl.UInt(bits = 32), expr = gray[(sum_x_s1_win2__y + sum_x_s1_y), (sum_x_s1_win2__x + sum_x_s1_x)]) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((hcl.cast(dtype = hcl.Int(bits = 32), expr = 4) - sum_x_s1_win2__x) * (sum_x_s1_win2__x + hcl.cast(dtype = hcl.Int(bits = 32), expr = -4)))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794))))) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((hcl.cast(dtype = hcl.Int(bits = 32), expr = 4) - sum_x_s1_win2__y) * (sum_x_s1_win2__y + hcl.cast(dtype = hcl.Int(bits = 32), expr = -4)))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794)))))
        ))
    sum_x_1 = hcl.compute((output_extent_1, output_extent_0, ), lambda sum_x_s1_y, sum_x_s1_x, : sum_x_1(sum_x_s1_y, sum_x_s1_x, input, sum_x_0, gray_0, ), name = "sum_x_1", dtype = hcl.UInt(bits = 32))

    def blur_x_0(blur_x_s0_y, blur_x_s0_x, input, sum_x, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (sum_x[blur_x_s0_y, blur_x_s0_x]/hcl.cast(dtype = hcl.UInt(bits = 32), expr = 65536)))
    blur_x_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda blur_x_s0_y, blur_x_s0_x, : blur_x_0(blur_x_s0_y, blur_x_s0_x, input, sum_x_1, ), name = "blur_x_0", dtype = hcl.UInt(bits = 8))

    def sharpen_0(sharpen_s0_y, sharpen_s0_x, input, blur_x, gray, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = gray[sharpen_s0_y, sharpen_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 2)) - hcl.cast(dtype = hcl.UInt(bits = 16), expr = blur_x[sharpen_s0_y, sharpen_s0_x])) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = gray[sharpen_s0_y, sharpen_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 2)) - hcl.cast(dtype = hcl.UInt(bits = 16), expr = blur_x[sharpen_s0_y, sharpen_s0_x])), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255)))
    sharpen_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda sharpen_s0_y, sharpen_s0_x, : sharpen_0(sharpen_s0_y, sharpen_s0_x, input, blur_x_0, gray_0, ), name = "sharpen_0", dtype = hcl.UInt(bits = 8))

    def ratio_0(ratio_s0_y, ratio_s0_x, input, sharpen, gray, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = sharpen[ratio_s0_y, ratio_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = hcl.select(gray[ratio_s0_y, ratio_s0_x] > hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1), gray[ratio_s0_y, ratio_s0_x], hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1)))) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = sharpen[ratio_s0_y, ratio_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = hcl.select(gray[ratio_s0_y, ratio_s0_x] > hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1), gray[ratio_s0_y, ratio_s0_x], hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1)))), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255)))
    ratio_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda ratio_s0_y, ratio_s0_x, : ratio_0(ratio_s0_y, ratio_s0_x, input, sharpen_0, gray_0, ), name = "ratio_0", dtype = hcl.UInt(bits = 8))

    def hw_output_0(hw_output_s0_c, hw_output_s0_y, hw_output_s0_x, input, ratio, ):
        return hcl.select(((input[hw_output_s0_c, (hw_output_s0_y + hcl.cast(dtype = hcl.Int(bits = 32), expr = 4)), (hw_output_s0_x + hcl.cast(dtype = hcl.Int(bits = 32), expr = 4))] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = ratio[hw_output_s0_y, hw_output_s0_x]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32)) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((input[hw_output_s0_c, (hw_output_s0_y + hcl.cast(dtype = hcl.Int(bits = 32), expr = 4)), (hw_output_s0_x + hcl.cast(dtype = hcl.Int(bits = 32), expr = 4))] * hcl.cast(dtype = hcl.UInt(bits = 16), expr = ratio[hw_output_s0_y, hw_output_s0_x]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32)), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255))
    hw_output_0 = hcl.compute((output_extent_2, output_extent_1, output_extent_0, ), lambda hw_output_s0_c, hw_output_s0_y, hw_output_s0_x, : hw_output_0(hw_output_s0_c, hw_output_s0_y, hw_output_s0_x, input, ratio_0, ), name = "hw_output_0", dtype = hcl.UInt(bits = 16))

    def output_0(output_s0_c, output_s0_y, output_s0_x, input, hw_output, ):
        return hw_output[output_s0_c, output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_2, output_extent_1, output_extent_0, ), lambda output_s0_c, output_s0_y, output_s0_x, : output_0(output_s0_c, output_s0_y, output_s0_x, input, hw_output_0, ), name = "output_0", dtype = hcl.UInt(bits = 16))

    return output_0
input = hcl.placeholder((3, 16, 16, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (2, 1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (3, 8, 8)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (2, 1, 0))
np.save("output_heterocl_uint.npy", np_out)
print(hcl.build(s, target = "soda"))
