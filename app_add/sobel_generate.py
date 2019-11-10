import heterocl as hcl
hcl.init()
output_extent_0 = 6478
output_extent_1 = 4818
output_min_0 = 0
output_min_1 = 0
def _all(input, filter_x, filter_y, ):
    def gx_0(gx_s0_y, gx_s0_x, input, filter_x, filter_y, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = 0)
    gx_0 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda gx_s0_y, gx_s0_x, : gx_0(gx_s0_y, gx_s0_x, input, filter_x, filter_y, ), name = "gx_0", dtype = hcl.UInt(bits = 8))

    def gx_1(gx_s1_y, gx_s1_x, input, filter_x, filter_y, gx, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        gx_s1_r__x = hcl.reduce_axis(0, 3)
        gx_s1_r__y = hcl.reduce_axis(0, 3)
        return (gx[gx_s1_y, gx_s1_x] + _sum(
            axis = [gx_s1_r__y, gx_s1_r__x, ],
            expr = (filter_x[gx_s1_r__y, gx_s1_r__x] * input[(gx_s1_r__y + gx_s1_y), (gx_s1_r__x + gx_s1_x)])
        ))
    gx_1 = hcl.compute(((output_extent_1 + 2), (output_extent_0 + 2), ), lambda gx_s1_y, gx_s1_x, : gx_1(gx_s1_y, gx_s1_x, input, filter_x, filter_y, gx_0, ), name = "gx_1", dtype = hcl.UInt(bits = 8))

    def gy_0(gy_s0_y, gy_s0_x, input, filter_x, filter_y, ):
        return hcl.cast(dtype = hcl.UInt(bits = 8), expr = 0)
    gy_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda gy_s0_y, gy_s0_x, : gy_0(gy_s0_y, gy_s0_x, input, filter_x, filter_y, ), name = "gy_0", dtype = hcl.UInt(bits = 8))

    def gy_1(gy_s1_y, gy_s1_x, input, filter_x, filter_y, gy, gx, ):
        _sum = hcl.reducer(0, lambda x, y: x + y)
        gy_s1_r__x = hcl.reduce_axis(0, 3)
        gy_s1_r__y = hcl.reduce_axis(0, 3)
        return (gy[gy_s1_y, gy_s1_x] + _sum(
            axis = [gy_s1_r__y, gy_s1_r__x, ],
            expr = (filter_y[gy_s1_r__y, gy_s1_r__x] * gx[(gy_s1_r__y + gy_s1_y), (gy_s1_r__x + gy_s1_x)])
        ))
    gy_1 = hcl.compute((output_extent_1, output_extent_0, ), lambda gy_s1_y, gy_s1_x, : gy_1(gy_s1_y, gy_s1_x, input, filter_x, filter_y, gy_0, gx_1, ), name = "gy_1", dtype = hcl.UInt(bits = 8))

    def output_0(output_s0_y, output_s0_x, input, filter_x, filter_y, gy, ):
        return gy[output_s0_y, output_s0_x]
    output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda output_s0_y, output_s0_x, : output_0(output_s0_y, output_s0_x, input, filter_x, filter_y, gy_1, ), name = "output_0", dtype = hcl.UInt(bits = 8))

    return output_0
input = hcl.placeholder((4820, 6480, ), name = "input", dtype = hcl.UInt(bits = 8))
filter_x = hcl.placeholder((3, 3, ), name = "filter_x", dtype = hcl.UInt(bits = 8))
filter_y = hcl.placeholder((3, 3, ), name = "filter_y", dtype = hcl.UInt(bits = 8))
s = hcl.create_schedule([input, filter_x, filter_y, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
# np_input = np.transpose(np.load("input.npy"), (1, 0))
# hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 8))
# np_filter_x = np.transpose(np.load("filter_x.npy"), (1, 0))
# hcl_filter_x = hcl.asarray(np_filter_x, dtype = hcl.UInt(bits = 8))
# np_filter_y = np.transpose(np.load("filter_y.npy"), (1, 0))
# hcl_filter_y = hcl.asarray(np_filter_y, dtype = hcl.UInt(bits = 8))
# output_shape = (4818, 6478)
# hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
# f(hcl_input, hcl_filter_x, hcl_filter_y, hcl_out)
# np_out = hcl_out.asnumpy()
# np_out = np.transpose(np_out, (1, 0))
# np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda")) # Load ... is not affine. Can't generate correct SODA code. But it should work for Merlin
# print(hcl.build(s, target = "merlinc")) # yes
