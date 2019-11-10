import heterocl as hcl
hcl.init()
__expo = hcl.local(init = 2.71828, dtype = hcl.Float())
_output_extent_0 = 198
_output_min_0 = 0
def _all(_input, ):
    def _output_0(_output_s0_x, _input, ):
        _t22 = _input[(_output_s0_x + 2)]
        _t23 = _input[(_output_s0_x + 1)]
        return ((((_input[_output_s0_x] + _t23) * 2) + (_t22 + _t23)) + _t22)
    _output_0 = hcl.compute((_output_extent_0, ), lambda _output_s0_x, : _output_0(_output_s0_x, _input, ), name = "_output_0")

    return _output_0
_input = hcl.placeholder((200, ), name = "_input")
s = hcl.create_schedule([_input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (0))
hcl_input = hcl.asarray(np_input)
output_shape = (198)
hcl_out = hcl.asarray(np.zeros(output_shape))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (0))
np.save("output_heterocl.npy", np_out)

print(hcl.build(s, target = "soda"))
print(hcl.build(s, target = "merlinc"))
