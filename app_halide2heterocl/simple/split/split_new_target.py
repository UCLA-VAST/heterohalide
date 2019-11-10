import heterocl as hcl
hcl.init()

_output_min_0 = 0
_output_extent_0 = 10

def _output(_input):
    def _output_0(_output_s0_x_x_inner, _output_s0_x_x_outer, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y*10) + (_output_min_0 + (hcl.select(((_output_s0_x_x_outer*2) < (_output_extent_0 + -2)), (_output_s0_x_x_outer*2), (_output_extent_0 + -2)) + _output_s0_x_x_inner)))]*2)
        return out[0]
    _output = hcl.compute((2, 5, 8, ), lambda _output_s0_x_inner, _output_s0_x_outer, _output_s0_y : _output_0(_output_s0_x_inner, _output_s0_x_outer, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 8, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)

s_0 = _output._output_0
s[s_0].reorder(s_0.axis[2], s_0.axis[1], s_0.axis[0])


f = hcl.build(s)
print(hcl.lower(s))

import numpy as np 
np_in = np.load("/curr/jiajieli/app_halide2heterocl/simple/input.npy")
np_in = np.transpose(np_in, (1,0)).flatten()
hcl_in = hcl.asarray(np_in)
out_shape = (2, 5, 8) #Here just the output shape
hcl_out = hcl.asarray(np.zeros(out_shape))

f(hcl_in, hcl_out)
np_out = hcl_out.asnumpy()
np.save("/curr/jiajieli/app_halide2heterocl/simple/split/output_heterocl.npy", np_out)
