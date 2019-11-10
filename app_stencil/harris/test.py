import heterocl as hcl 
hcl.init()
def test(input):
    def divide(y, x, input):
        return input[y, x] / 144
        # return hcl.cast(dtype = hcl.Float(), expr = input[y, x] / 144)
    output = hcl.compute((4, 4), lambda y, x: divide(y, x, input), name = "divide")
    return output
input = hcl.placeholder((4, 4), name = "input", dtype = hcl.Int())
s = hcl.create_schedule([input], test)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np 
np_input = np.transpose(np.load("input_test.npy"), (1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.Int())
output_shape = (4, 4)
hcl_out = hcl.asarray(np.zeros(output_shape)) # dtype = hcl.UInt(16)) # dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
# np.save("test_float.npy", np_out)
np_real_input = np.load("input_test.npy")
print(np_real_input)
print(np_out)

test_halide = np.loadtxt("output_test.txt")
test_halide = np.transpose(test_halide.reshape((4, 4)), (1, 0)) # reshape: (h, w)
print(test_halide)