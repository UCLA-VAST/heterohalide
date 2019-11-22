import heterocl as hcl
hcl.init()
output_extent_0 = 8
output_extent_1 = 8
output_extent_2 = 3
output_min_0 = 0
output_min_1 = 0
output_min_2 = 0
def _all(input, ):
    allocate gray[uint8 * (output_extent_0 + 8) * (output_extent_1 + 8)]
    def gray_0(gray_s0_y, gray_s0_x, input, ):
hcl.cast(dtype = hcl.UInt(bits = 8), expr = (((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 29)) + ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 77)) + (hcl.cast(dtype = hcl.UInt(bits = 16), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 150))))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = 256)))    gray_0 = hcl.compute(((output_extent_1 + 8), (output_extent_0 + 8), ), lambda gray_s0_y, gray_s0_x, : gray_0(gray_s0_y, gray_s0_x, input, ), name = "gray_0", dtype = (bits = 0))

    allocate sum_x[uint32 * output_extent_0 * output_extent_1]
    def sum_x_0(sum_x_s0_y, sum_x_s0_x, input, ):
hcl.cast(dtype = hcl.UInt(bits = 32), expr = 0)    sum_x_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda sum_x_s0_y, sum_x_s0_x, : sum_x_0(sum_x_s0_y, sum_x_s0_x, input, ), name = "sum_x_0", dtype = (bits = 0))

    def sum_x_1(sum_x_s1_y, sum_x_s1_x, input, sum_x, gray, ):
( + (
            axis = [sum_x_s1_win2__y, sum_x_s1_win2__x, ],
            expr = ((hcl.cast(dtype = hcl.UInt(bits = 32), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__x) * (sum_x_s1_win2__x + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794))))) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__y) * (sum_x_s1_win2__y + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794)))))
        ))    sum_x_1 = hcl.compute((output_extent_1, output_extent_0, ), lambda sum_x_s1_y, sum_x_s1_x, : sum_x_1(sum_x_s1_y, sum_x_s1_x, input, sum_x_0, gray_0, ), name = "sum_x_1", dtype = (bits = 0))

    allocate blur_x[uint8 * output_extent_0 * output_extent_1]
    def blur_x_0(blur_x_s0_y, blur_x_s0_x, input, sum_x, ):
hcl.cast(dtype = hcl.UInt(bits = 8), expr = (/hcl.cast(dtype = hcl.UInt(bits = 32), expr = 65536)))    blur_x_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda blur_x_s0_y, blur_x_s0_x, : blur_x_0(blur_x_s0_y, blur_x_s0_x, input, sum_x_1, ), name = "blur_x_0", dtype = (bits = 0))

    free sum_x
    allocate sharpen[uint8 * output_extent_0 * output_extent_1]
    def sharpen_0(sharpen_s0_y, sharpen_s0_x, input, blur_x, gray, ):
hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 2)) - hcl.cast(dtype = hcl.UInt(bits = 16), expr = )) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 2)) - hcl.cast(dtype = hcl.UInt(bits = 16), expr = )), hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255)))    sharpen_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda sharpen_s0_y, sharpen_s0_x, : sharpen_0(sharpen_s0_y, sharpen_s0_x, input, blur_x_0, gray_0, ), name = "sharpen_0", dtype = (bits = 0))

    free blur_x
    allocate ratio[uint8 * output_extent_0 * output_extent_1]
    def ratio_0(ratio_s0_y, ratio_s0_x, input, sharpen, gray, ):
hcl.cast(dtype = hcl.UInt(bits = 8), expr = hcl.select(((hcl.cast(dtype = hcl.UInt(bits = 16), expr = ) * hcl.cast(dtype = hcl.UInt(bits = 16), expr = 32))/hcl.cast(dtype = hcl.UInt(bits = 16), expr = hcl.select( > hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1), , hcl.cast(dtype = hcl.UInt(bits = 8), expr = 1)))) < hcl.cast(dtype = hcl.UInt(bits = 16), expr = 255), ((hcl.cast(dtype 