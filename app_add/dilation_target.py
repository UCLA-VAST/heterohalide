import heterocl as hcl
hcl.init()
output_extent_0 = 6478
output_extent_1 = 4818
output_min_0 = 0
output_min_1 = 0
def _all(input, ):
    with hcl.Stage("S"):
        maximum = hcl.compute((output_extent_1, output_extent_0), lambda y, x: 0, name = "maximum_init")
        with hcl.for_(output_min_1, output_extent_1) as hw_output_s0_y:
            with hcl.for_(output_min_0, output_extent_0) as hw_output_s0_x:
                _sum = hcl.reducer(0, lambda x, y: x + y)
                maximum_s1_box__x = hcl.reduce_axis(0, 3)
                maximum_s1_box__y = hcl.reduce_axis(0, 3)   
                maximum[hw_output_s0_y, hw_output_s0_x] = hcl.max(axis = [maximum_s1_box__y, maximum_s1_box__x],
                        expr = input[hw_output_s0_y + maximum_s1_box__y, hw_output_s0_x + maximum_s1_box__x]
                        )

s = hcl.create_schedule([input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))


# When using hcl.for_: raise DSLError("Imperative DSL must be used with other compute APIs")



#     def hw_output_0(hw_output_s0_y, hw_output_s0_x, input, ):
#         return hcl.cast(dtype = hcl.UInt(bits = 8), expr = 0)
#         def maximum_0(hw_output_s0_y, hw_output_s0_x, input, ):
#             _sum = hcl.reducer(0, lambda x, y: x + y)
#             maximum_s1_box__x = hcl.reduce_axis(0, 3)
#             maximum_s1_box__y = hcl.reduce_axis(0, 3)
#             return hcl.select(maximum[] > input[(hw_output_s0_y + _sum(
#                 axis = [maximum_s1_box__y, maximum_s1_box__x, ],
#                 expr = maximum_s1_box__y
#             )), (hw_output_s0_x + maximum_s1_box__x)], maximum[], input[(hw_output_s0_y + maximum_s1_box__y), (hw_output_s0_x + maximum_s1_box__x)])
#         maximum_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda hw_output_s0_y, hw_output_s0_x, : maximum_0(hw_output_s0_y, hw_output_s0_x, input, ), name = "maximum_0", dtype = hcl.UInt(bits = 8))

#         return maximum[hw_output_s0_y, hw_output_s0_x]
#     maximum_0 = hcl.compute((), lambda : maximum_0(input, hw_output_0, maximum_0, ), name = "maximum_0", dtype = hcl.UInt(bits = 8))

#     def output_0(output_s0_y, output_s0_x, input, hw_output, ):
#         return hw_output[]
#     output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda output_s0_y, output_s0_x, : output_0(output_s0_y, output_s0_x, input, hw_output_0, ), name = "output_0", dtype = hcl.UInt(bits = 8))

#     return output_0
# input = hcl.placeholder((4820, 6480, ), name = "input", dtype = hcl.UInt(bits = 8))
# s = hcl.create_schedule([input, ], _all)
# f = hcl.build(s)
# print(hcl.lower(s))
# import numpy as np
# np_input = np.transpose(np.load("input.npy"), (1, 0))
# hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 8))
# output_shape = (4818, 6478)
# hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 8))
# f(hcl_input, hcl_out)
# np_out = hcl_out.asnumpy()
# np_out = np.transpose(np_out, (1, 0))
# np.save("output_heterocl.npy", np_out)
# print(hcl.build(s, target = "soda"))
