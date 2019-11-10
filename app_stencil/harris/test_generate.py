import heterocl as hcl
hcl.init()
output_extent_0 = 4
output_extent_1 = 4
output_min_0 = 0
output_min_1 = 0
def _all(input, ):
    def output_0(output_s0_y, output_s0_x, input, ):
        return (input[output_s0_y, output_s0_x]/144)
    output_0 = hcl.compute((output_extent_1, output_extent_0, ), lambda output_s0_y, output_s0_x, : output_0(output_s0_y, output_s0_x, input, ), name = "output_0", dtype = (bits = 0))

    return output_0
input = hcl.placeholder((4, 4, ), name = "input")
s = hcl.create_schedule([input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (1, 0))
hcl_input = hcl.asarray(np_input)
output_shape = (4, 4)
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = (bits = 0))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl.npy", np_out)
