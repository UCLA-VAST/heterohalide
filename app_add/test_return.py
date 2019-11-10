import heterocl as hcl 
hcl.init()

input = hcl.placeholder((4,32,10,10), "input")
filter = hcl.placeholder((32,32,3,3), "filter")
bias = hcl.placeholder((32,), "bias")
print(bias.shape)

def conv_layer(input, filter, bias):
    def compare_with_zero(a):
        with hcl.if_(a > 0):
            hcl.return_(a)
        with hcl.else_():
            hcl.return_(0)
    output = hcl.compute(input.shape, lambda n,c,h,w: compare_with_zero(input[n,c,h,w]), "output")
    return output
s = hcl.create_schedule([input, filter, bias], conv_layer)
print(hcl.lower(s))
f = hcl.build(s)
print('done')

# Works well. The problem is fixed with upgraded HeteorCL version.