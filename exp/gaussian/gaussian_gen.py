import heterocl as hcl
hcl.init()
final_extent_0 = 392
final_extent_1 = 592
final_min_0 = 0
final_min_1 = 0
def top(input, ):
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    blur_x = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "blur_x", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("blur_x"):
        with hcl.for_(final_min_1, final_extent_1, name = "blur_x_s0_y") as blur_x_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "blur_x_s0_x") as blur_x_s0_x:
                sum_x = hcl.compute((1, 1), lambda x, y: 0, name = "sum_x", dtype = hcl.UInt(bits = 32))
                with hcl.Stage("sum_x"):
                    sum_x[blur_x_s0_x, blur_x_s0_y] = hcl.cast(dtype = hcl.UInt(bits = 32), expr = 0)
                    with hcl.for_(0, 9, name = "sum_x_s1_win2__y") as sum_x_s1_win2__y:
                        with hcl.for_(0, 9, name = "sum_x_s1_win2__x") as sum_x_s1_win2__x:
                            sum_x[blur_x_s0_x, blur_x_s0_y] = (sum_x[blur_x_s0_x, blur_x_s0_y] + ((hcl.cast(dtype = hcl.UInt(bits = 32), expr = input[(blur_x_s0_x + sum_x_s1_win2__x), (blur_x_s0_y + sum_x_s1_win2__y)]) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__x) * (sum_x_s1_win2__x + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794))))) * hcl.cast(dtype = hcl.UInt(bits = 32), expr = hcl.cast(dtype = hcl.UInt(bits = 8), expr = (hcl.call_pure_intrin("float", "pow", 2.718280, (hcl.cast(dtype = hcl.Float(bits = 32), expr = ((4 - sum_x_s1_win2__y) * (sum_x_s1_win2__y + -4))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.222222))) * hcl.cast(dtype = hcl.Float(bits = 32), expr = 67.972794))))))
                blur_x[blur_x_s0_x, blur_x_s0_y] = hcl.cast(dtype = hcl.UInt(bits = 16), expr = (sum_x[blur_x_s0_x, blur_x_s0_y]//hcl.cast(dtype = hcl.UInt(bits = 32), expr = 65536)))
    output_final = hcl.compute((final_extent_0, final_extent_1), lambda x, y: 0, name = "output_final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("output_final"):
        with hcl.for_(final_min_1, final_extent_1, name = "output_final_s0_y") as output_final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "output_final_s0_x") as output_final_s0_x:
                output_final[output_final_s0_x, output_final_s0_y] = blur_x[output_final_s0_x, output_final_s0_y]
    final = hcl.compute((392, 592), lambda x, y: 0, name = "final", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_y") as final_s0_y:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                final[final_s0_x, final_s0_y] = output_final[final_s0_x, final_s0_y]
    return final
input = hcl.placeholder((400, 600, ), name = "input", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, ], top)
f = hcl.build(s)
print(hcl.lower(s))
import numpy as np
np_input = np.load("input.npy")
hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
output_shape = (392, 592, )
hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.UInt(bits = 16))
f(hcl_input, hcl_out)
np_out = hcl_out.asnumpy()
np.save("output_heterocl.npy", np_out)
print(hcl.build(s, target = "soda"))
