import heterocl as hcl
hcl.init()
final_extent_0 = 10
final_extent_1 = 10
final_min_0 = 0
final_min_1 = 0
def _all(input, ):
    def final_0(final_s0_y, final_s0_x, input, ):
        return (input[final_s0_y, final_s0_x] * 2)
    final_0 = hcl.compute((final_extent_1, final_extent_0, ), lambda final_s0_y, final_s0_x, : final_0(final_s0_y, final_s0_x, input, ), name = "final_0")

    return final_0
input = hcl.placeholder((10, 10, ), name = "input", dtype = hcl.Int(bits = 32))
s = hcl.create_schedule([input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.transpose(np.load("input.npy"), (1, 0))
hcl_input = hcl.asarray(np_input, dtype = hcl.Int(bits = 32))
output_shape = (10, 10)
hcl_out = hcl.asarray(np.zeros(output_shape))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np_out = np.transpose(np_out, (1, 0))
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
