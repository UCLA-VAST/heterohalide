# HeteroCL doesn't support return_ statement inside a normal Python function call. 
import heterocl as hcl 
import heterocl.tvm as tvm
hcl.init()


import heterocl.tvm as tvm

dtype = hcl.Float()

sum = hcl.reducer(0, lambda x, y: x + y, dtype)

def simplify(expr):
    return tvm.ir_pass.Simplify(expr) if isinstance(expr, tvm.expr.Expr) else expr

def conv2d_nchw(Input, Filter, name="conv2d", stride=[1,1], padding=[[0,0],[0,0]]):
    out_dtype = Input.dtype
    # batch, in_channel, in_height, in_width = Input.shape
    in_width, in_height, in_channel, batch = Input.shape
    # num_filter, channel, kernel_h, kernel_w = Filter.shape
    kernel_w, kernel_h, channel, num_filter = Filter.shape
    stride_h, stride_w = stride
    [pad_top, pad_left], [pad_down, pad_right] = padding
    # compute the output shape
    out_channel = num_filter
    out_height = simplify((in_height - kernel_h + pad_top + pad_down) // stride_h + 1)
    out_width = simplify((in_width - kernel_w + pad_left + pad_right) // stride_w + 1)
    # compute graph
    pad_before = [0, 0, pad_top, pad_left]
    pad_after = [0, 0, pad_down, pad_right]
    # if padding != [[0,0],[0,0]]:
    #     Input = pad(Input, pad_before, pad_after)
    rc = hcl.reduce_axis(0, in_channel)
    ry = hcl.reduce_axis(0, kernel_h)
    rx = hcl.reduce_axis(0, kernel_w)

    return hcl.compute(
        (out_width, out_height, out_channel, batch),
        lambda xx, yy, ff, nn: sum(
            Input[xx * stride_w + rx, yy * stride_h + ry, rc, nn] * Filter[rx, ry, rc, ff],
            axis = [rx, ry, rc]),
        name = "conv2d", 
    )

input = hcl.placeholder((67, 67, 32, 4), name = "input") #(w,h,c,n)
filter = hcl.placeholder((3, 3, 32, 32), name = "filter")
bias = hcl.placeholder((32, ), name = "bias")

def conv_layer(input, filter, bias):
    def compare_with_zero(a):
        return hcl.select(a > 0, a, 0)

    conv1 = conv2d_nchw(input, filter)
    conv_with_bias = hcl.compute(conv1.shape, lambda w, h, c, n: conv1[w, h, c, n] + bias[c], name = "conv_with_bias")
    output = hcl.compute(conv1.shape, lambda w, h, c, n: compare_with_zero(conv_with_bias[w, h, c, n]), name = "output") #Segmentation fault: some bug of HeteroCL(?): when placeholder's dim is 2, it works
    return output

s = hcl.create_schedule([input, filter, bias], conv_layer)

print(hcl.lower(s))

f = hcl.build(s)
print('done')

import numpy as np
np_in = np.load("input.npy")
np_filter = np.load("filter.npy")
np_bias = np.load("bias.npy")

hcl_in = hcl.asarray(np_in)
hcl_filter = hcl.asarray(np_filter)
hcl_bias = hcl.asarray(np_bias)

out_shape = (65, 65, 32, 4) #Actually, it's (4,32,65,65). If slice too much, the memory error may occur. 
hcl_out = hcl.asarray(np.zeros(out_shape))
f(hcl_in, hcl_filter, hcl_bias, hcl_out)

np_out = hcl_out.asnumpy()
np_out_64 = np_out[0:64, 0:64, :, :]
np.save("/curr/jiajieli/app_halide2heterocl/conv/output_heterocl.npy", np_out_64)