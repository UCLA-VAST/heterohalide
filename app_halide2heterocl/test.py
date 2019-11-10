import heterocl as hcl
import heterocl.tvm as tvm
hcl.init()
input = hcl.placeholder((200,300), "in")
bias = hcl.placeholder((10,), "bias")
print(bias.shape)

out_shape = (5,10)

def test(input, bias):
    def compare_with_zero(a):
        with hcl.if_(a > 0):
            hcl.return_(a)
        with hcl.else_():
            hcl.return_(0)
    out = hcl.compute(input.shape, lambda x,y: compare_with_zero(input[x,y]), "output")
    return out

s = hcl.create_schedule([input, bias], test)

print(hcl.lower(s))
f = hcl.build(s)

print("as")

import numpy as np 
np_in = np.random.randint(-5,5,size = input.shape)
hcl_in = hcl.asarray(np_in)
np_bias = np.random.randint(3, size=bias.shape)
hcl_bias = hcl.asarray(np_bias)

hcl_tmp = hcl.asarray(np.zeros((200,300)))

f(hcl_in, hcl_bias, hcl_tmp)

np_in = hcl_in.asnumpy()
np_tmp = hcl_tmp.asnumpy()

print(np_in)
print(np_tmp)

