import heterocl as hcl 
hcl.init()


_output_extent_0 = 8
_output_extent_1 = 8
# def simple(input):
#     output = hcl.compute((8, 8), lambda x, y: input[x, y] * 2, name = "output")
#     return output
def output(input):
    output = hcl.compute((_output_extent_0, _output_extent_1), lambda _output_s0_x, _output_s0_y: (input[((_output_s0_y * 8) + _output_s0_x)] * 2))
    return output

input = hcl.placeholder((64,), name = "input")

s = hcl.create_schedule([input], output)
f = hcl.build(s)

print(hcl.lower(s))
