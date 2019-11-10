# need to repeat the operation

import heterocl as hcl 
import numpy as np 

hcl.init()
input = hcl.placeholder((2560, 1536), "input", dtype = hcl.UInt(bits = 16))
# iterations = 2 #num of iterations
# according to Halide program: stencil_chain/process.cpp, just take the red channel

def stencil_chain(input):
    def clamp(x, low, high):
        return hcl.select(x <= high, hcl.select(x >= low, x, low), high)
        # out = hcl.local(0)
        # with hcl.if_(x > high):
        #     out[0] = high
        # with hcl.elif_(x < low):
        #     out[0] = low
        # with hcl.else_():
        #     out[0] = x
        # return out[0]

    sum = hcl.reducer(0, lambda x, y: x + y)
    rx = hcl.reduce_axis(0, 5) # -2 <= rx <= 2
    ry = hcl.reduce_axis(0, 5) # -2 <= ry <= 2
    output = hcl.compute(input.shape, # need to deal with boundaries: repeat_edge
                lambda x, y: sum(
                    input[clamp(x + rx - 2, 0, input.shape[0] - 1), clamp(y + ry - 2, 0, input.shape[1] - 1)] * (rx + 1) * (ry + 1), 
                    # didn't work, error: Check failed: !use_count_.count(v) variable ra0 has been used before definition!
                    axis = [rx, ry]),
                name = "conv",
                dtype = hcl.UInt(16)
                )
    return output
    

s = hcl.create_schedule([input], stencil_chain)
print(hcl.lower(s))
f = hcl.build(s)
# f = hcl.build(s, target = 'vhls')

print('done')

np_in = np.load("/curr/jiajieli/app_halide2heterocl/stencil/input.npy")
hcl_in = hcl.asarray(np_in, dtype = hcl.UInt(bits = 16))

hcl_out = hcl.asarray(np.zeros(input.shape).astype('uint16'), dtype = hcl.UInt(bits = 16)) # if the output_shape we claim is input.shape, probably memory error will occur. Should cut it in numpy array

# hcl_out = hcl.asarray(np.zeros(input.shape).astype('uint16'), dtype = hcl.UInt(bits = 16))

# hcl_in = hcl.asarray(np_in)
# hcl_out = hcl.asarray(np.zeros((2500, 1500)))

f(hcl_in, hcl_out)

np_out = hcl_out.asnumpy()

# np_out_cut = np.reshape( np_out.flatten()[0 : input.shape[0] * input.shape[1]], input.shape )
np.save("/curr/jiajieli/app_halide2heterocl/stencil/output_heterocl_1.npy", np_out)