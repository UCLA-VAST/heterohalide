import heterocl as hcl
hcl.init()
output_1_extent_0 = 8
output_1_extent_1 = 8
output_1_min_0 = 0
output_1_min_1 = 0
def _all(input_0, input_1, input_2, ):
    def gray_0(gray_s0_y, gray_s0_x, input_0, input_1, input_2, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (((hcl.cast(dtype = hcl.UInt(bits = 16), expr = input_2[gray_s0_y, gray_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 29)) + ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = input_0[gray_s0_y, gray_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 77)) + (hcl.cast(dtype = hcl.UInt(bits = 16), expr = input_1[gray_s0_y, gray_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 150))))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 256)))
    gray_0 = hcl.compute(((output_1_extent_1 + 8), (output_1_extent_0 + 8), ), lambda gray_s0_y, gray_s0_x, : gray_0(gray_s0_y, gray_s0_x, input_0, input_1, input_2, ), name = "gray_0", dtype = hcl.UInt(bits = 8))

    def sum_x_0(sum_x_s0_y, sum_x_s0_x, input_0, input_1, input_2, ):
        return hcl.cast(dtype = hcl.UInt(bits = 32), expr = 0)
    sum_x_0 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda sum_x_s0_y, sum_x_s0_x, : sum_x_0(sum_x_s0_y, sum_x_s0_x, input_0, input_1, input_2, ), name = "sum_x_0", dtype = hcl.UInt(bits = 32))

    def sum_x_1(sum_x_s1_y, sum_x_s1_x, input_0, input_1, input_2, sum_x, gray, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        sum_x_s1_win2__x = hcl.reduce_axis(0, 9)
        sum_x_s1_win2__y = hcl.reduce_axis(0, 9)
        return (sum_x[sum_x_s1_y, sum_x_s1_x] + _sum(
            axis = [sum_x_s1_win2__y, sum_x_s1_win2__x, ],
            expr = ((hcl.cast(dtype = hcl.UInt(bits = 32), expr = gray[(sum_x_s1_win2__y + sum_x_s1_y), (sum_x_s1_win2__x + sum_x_s1_x)]) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__x) * (sum_x_s1_win2__x + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794))))) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__y) * (sum_x_s1_win2__y + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794)))))
        ))
    sum_x_1 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda sum_x_s1_y, sum_x_s1_x, : sum_x_1(sum_x_s1_y, sum_x_s1_x, input_0, input_1, input_2, sum_x_0, gray_0, ), name = "sum_x_1", dtype = hcl.UInt(bits = 32))

    def blur_x_0(blur_x_s0_y, blur_x_s0_x, input_0, input_1, input_2, sum_x, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = (sum_x[blur_x_s0_y, blur_x_s0_x]/hcl.cast(dtype = hcl.UInt(bits = 32), expr = 65536)))
    blur_x_0 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda blur_x_s0_y, blur_x_s0_x, : blur_x_0(blur_x_s0_y, blur_x_s0_x, input_0, input_1, input_2, sum_x_1, ), name = "blur_x_0", dtype = hcl.UInt(bits = 8))

    def sharpen_0(sharpen_s0_y, sharpen_s0_x, input_0, input_1, input_2, blur_x, gray, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = gray[sharpen_s0_y, sharpen_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 2)) - hcl.cast(dtype = hcl.UInt(bits = 16), expr = blur_x[sharpen_s0_y, sharpen_s0_x])) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = gray[sharpen_s0_y, sharpen_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 2)) - hcl.cast(dtype = hcl.UInt(bits = 16), expr = blur_x[sharpen_s0_y, sharpen_s0_x])), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255)))
    sharpen_0 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda sharpen_s0_y, sharpen_s0_x, : sharpen_0(sharpen_s0_y, sharpen_s0_x, input_0, input_1, input_2, blur_x_0, gray_0, ), name = "sharpen_0", dtype = hcl.UInt(bits = 8))

    def ratio_0(ratio_s0_y, ratio_s0_x, input_0, input_1, input_2, sharpen, gray, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = sharpen[ratio_s0_y, ratio_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = hcl.select(gray[ratio_s0_y, ratio_s0_x] > hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1), gray[ratio_s0_y, ratio_s0_x], hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1)))) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = sharpen[ratio_s0_y, ratio_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = hcl.select(gray[ratio_s0_y, ratio_s0_x] > hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1), gray[ratio_s0_y, ratio_s0_x], hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1)))), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255)))
    ratio_0 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda ratio_s0_y, ratio_s0_x, : ratio_0(ratio_s0_y, ratio_s0_x, input_0, input_1, input_2, sharpen_0, gray_0, ), name = "ratio_0", dtype = hcl.UInt(bits = 8))

    def hw_output_1_0(hw_output_1_s0_y, hw_output_1_s0_x, input_0, input_1, input_2, ratio, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ratio[hw_output_1_s0_y, hw_output_1_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = input_1[(hw_output_1_s0_y + 4), (hw_output_1_s0_x + 4)]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32)) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ratio[hw_output_1_s0_y, hw_output_1_s0_x]) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = input_1[(hw_output_1_s0_y + 4), (hw_output_1_s0_x + 4)]))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32)), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255)))
    hw_output_1_0 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda hw_output_1_s0_y, hw_output_1_s0_x, : hw_output_1_0(hw_output_1_s0_y, hw_output_1_s0_x, input_0, input_1, input_2, ratio_0, ), name = "hw_output_1_0", dtype = hcl.UInt(bits = 8))

    def output_1_0(output_1_s0_y, output_1_s0_x, input_0, input_1, input_2, hw_output_1, ):
        return hw_output_1[output_1_s0_y, output_1_s0_x]
    output_1_0 = hcl.compute((output_1_extent_1, output_1_extent_0, ), lambda output_1_s0_y, output_1_s0_x, : output_1_0(output_1_s0_y, output_1_s0_x, input_0, input_1, input_2, hw_output_1_0, ), name = "output_1_0", dtype = hcl.UInt(bits = 8))

    return output_1_0
input_0 = hcl.placeholder((16, 16, ), name = "input_0", dtype = hcl.UInt(bits = 8))
input_1 = hcl.placeholder((16, 16, ), name = "input_1", dtype = hcl.UInt(bits = 8))
input_2 = hcl.placeholder((16, 16, ), name = "input_2", dtype = hcl.UInt(bits = 8))
s = hcl.create_schedule([input_0, input_1, input_2, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input_0 = np.transpose(np.load("input_0.npy"), (1, 0))
hcl_input_0 = hcl.asarray(np_input_0, dtype = hcl.UInt(bits = 8))
np_input_1 = np.transpose(np.load("input_1.npy"), (1, 0))
hcl_input_1 = hcl.asarray(np_input_1, dtype = hcl.UInt(bits = 8))
np_input_2 = np.transpose(np.load("input_2.npy"), (1, 0))
hcl_input_2 = hcl.asarray(np_input_2, dtype = hcl.UInt(bits = 8))
output_shape = (8, 8)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
f(hcl_input_0, hcl_input_1, hcl_input_2, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl_1.npy", np_out)
print(hcl.build(s, target = "soda"))
