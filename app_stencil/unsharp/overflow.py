import heterocl as hcl 
hcl.init()
def test(input):
    def overflow(y, x, input):
        return input[y, x] + hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - x) * (x + -4))) * float(0.222222)))
    output = hcl.compute((4, 4), lambda y, x: overflow(y, x, input), name = "overflow_test")
    return output
input = hcl.placeholder((4, 4), name = "input")
s = hcl.create_schedule([input, ], test)
print(hcl.lower(s))
f = hcl.build(s)
print("build success")
# import numpy as np 
# output_shape = (4, 4)
# hcl_out = hcl.asarray(np.zeros(output_shape))
# f(hcl_out)
# np_out = hcl_out.asnumpy()
# print(np_out)
print(hcl.build(s, target = "soda"))
print("soda generated")