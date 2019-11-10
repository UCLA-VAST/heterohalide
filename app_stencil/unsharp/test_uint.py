import heterocl as hcl 
hcl.init()
def test(input_1, input_2):
    def test_uint(y, x, input_1, input_2):# hcl.cast(dtype = hcl.UInt(16), expr = 2)
        return hcl.cast(dtype = hcl.UInt(16), expr = input_1[y, x]) * hcl.cast(dtype = hcl.UInt(8), expr = 2) - hcl.cast(dtype = hcl.UInt(16), expr = input_2[y, x]) 
        # return hcl.cast(expr = input_1[y, x] - input_2[y, x], dtype = hcl.UInt(bits = 16))
    output = hcl.compute((4, 4), lambda y, x : test_uint(y, x, input_1, input_2), name = "test_uint") #, dtype = hcl.UInt(bits = 16))
    return output
input_1 = hcl.placeholder((4, 4), dtype = hcl.UInt(bits = 16), name = "input_1")
input_2 = hcl.placeholder((4, 4), dtype = hcl.UInt(bits = 16), name = "input_2")

s = hcl.create_schedule([input_1, input_2, ], test)
f = hcl.build(s)
print(hcl.lower(s))
# import numpy as np
# np_input_real_1 = np.load("input_test_1.npy")
# print(np_input_real_1)
# np_input_real_2 = np.load("input_test_2.npy")
# print(np_input_real_2)
# np_input_1 = np.transpose(np_input_real_1, (1, 0))
# hcl_input_1 = hcl.asarray(np_input_1, dtype = hcl.UInt(bits = 16))
# np_input_2 = np.transpose(np_input_real_2, (1, 0))
# hcl_input_2 = hcl.asarray(np_input_2, dtype = hcl.UInt(bits = 16))
# output_shape = (4, 4)
# hcl_out = hcl.asarray(np.zeros(output_shape)) #, dtype = hcl.UInt(bits = 16))
# f(hcl_input_1, hcl_input_2, hcl_out)
# np_out = hcl_out.asnumpy()
# np_out = np.transpose(np_out, (1, 0))
# print(np_out)
# output_test_halide = np.loadtxt("output_test_0_halide.txt")
# output_test_halide = np.transpose(output_test_halide.reshape((4, 4)), (1, 0))
# print(output_test_halide)
print(hcl.build(s, target = "soda"))