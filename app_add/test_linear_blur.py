import heterocl as hcl 
hcl.init()
output_extent_0 = 766
output_extent_1 = 1278
output_extent_2 = 3
output_min_0 = 0
output_min_1 = 0
output_min_2 = 0
input = hcl.placeholder((768, 1280, 3, ), name = "input", dtype = hcl.Float(bits = 32))
def top(input):
    linear = hcl.compute((768, 1280, 3), lambda x, y, c: 0, name = "linear", dtype = hcl.Float(bits = 32))
    with hcl.Stage("linear"):
        with hcl.for_(output_min_2, output_extent_2, name = "linear_s0_c") as linear_s0_c:
            with hcl.for_(output_min_1, output_extent_1, name = "linear_s0_y") as linear_s0_y: 
                with hcl.for_(output_min_0, output_extent_0, name = "linear_s0_x") as linear_s0_x:
                    t8 = input[linear_s0_x, linear_s0_y, linear_s0_c]
                    linear[linear_s0_x, linear_s0_y, linear_s0_c] = hcl.select((hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.040450) < t8), hcl.power(((t8 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.947867)) + hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.052133)), hcl.cast(dtype = hcl.Float(bits = 32), expr = 2.400000)), (t8 * hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.077399)))
    return linear

s = hcl.create_schedule([input], top)
print(hcl.lower(s))