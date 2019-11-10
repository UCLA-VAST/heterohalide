import heterocl as hcl
hcl.init()
input = hcl.placeholder((648, 482), "in") #w = 648, h = 482


def blur(input):
    tmp = hcl.compute((640,482), lambda x, y: ( input[x, y] + input[x+1, y] + input[x+2, y] )/3, "tmp")
    out = hcl.compute((640,480), lambda x, y: ( tmp[x, y] + tmp[x, y+1] + tmp[x, y+2] )/3, "out")
    return out

s = hcl.create_schedule([input], blur)
f = hcl.build(s)

import numpy as np
# np_in = np.random.randint(100, size = input.shape)
np_in = np.load("input.npy")

hcl_in = hcl.asarray(np_in)
hcl_out = hcl.asarray(np.zeros((640,480)))

print(hcl.lower(s))

f(hcl_in, hcl_out)

np_in = hcl_in.asnumpy()
np_out = hcl_out.asnumpy()

# print(np_in.shape)
# print(np_out.shape)
# print(np_in)
# print(np_out)
np.save('/curr/jiajieli/app_halide2heterocl/blur/output_heterocl.npy', np_out)