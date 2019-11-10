#linear_blur: srgb to linear, simple blur, linear to srgb
import heterocl as hcl 
import numpy as np 
hcl.init()
# hcl.init(hcl.Float())

input = hcl.placeholder((768, 1280, 3), "input", dtype = hcl.Float()) #input shape = output shape, (w, h, c)

def linear_blur(input):
    def sRGBtoLinear(srgb):
        linear = hcl.local(0, dtype = hcl.Float())
        with hcl.if_(srgb <= 0.04045):
            linear[0] = srgb / 12.92
        with hcl.else_():
            # linear[0] = ((srgb + 0.055) / 1.055 ) ** 2.4
            linear[0] = hcl.power((srgb + 0.055) / 1.055 , 2.4)
        return linear[0]
    linear_tmp = hcl.compute(input.shape, lambda x,y,c: sRGBtoLinear(input[x,y,c]), "sRGBtoLinear", dtype = hcl.Float())

    # Halide:BoundaryConditions::repeat_edge   means the edge have the same value with the pixel next to it? 
    # e.g. blur_tmp[1280,768,0] = (blur_tmp[1280,768,0]+blur_tmp[1281,768,0]+blur_tmp[1282,768,0]) / 3 = blur_tmp[1280,768,0]?
    # didn't find the related source code in Halide
    def blur_x(linear_tmp, x, y, c):
        blurx = hcl.local(0, dtype = hcl.Float())
        with hcl.if_(x < input.shape[0] - 2): # x != 766 or 767 
            blurx[0] = (linear_tmp[x,y,c] + linear_tmp[x+1,y,c] + linear_tmp[x+2,y,c]) / 3
        with hcl.elif_(x == input.shape[0] - 2): # x = 766 
            blurx[0] = (linear_tmp[x,y,c] + linear_tmp[x+1,y,c] + linear_tmp[x+1,y,c]) / 3
        with hcl.else_(): # x = 767
            blurx[0] = linear_tmp[x,y,c]
        return blurx[0]
    blur_x = hcl.compute(input.shape, lambda x,y,c: blur_x(linear_tmp, x, y, c), "blur_x", dtype = hcl.Float())

    def blur_y(blur_x, linear_tmp, x, y, c):
        blury = hcl.local(0, dtype = hcl.Float())
        with hcl.if_(y < input.shape[1] - 2): # y != 1278 or 1279
            blury[0] = (blur_x[x,y,c] + blur_x[x,y+1,c] + blur_x[x,y+2,c]) / 3
        with hcl.elif_(y == input.shape[1] - 2): # y = 1278
            blury[0] = (blur_x[x,y,c] + blur_x[x,y+1,c] + linear_tmp[x,y+1,c]) / 3
        with hcl.else_(): # y = 1279
            blury[0] = (blur_x[x,y,c] + linear_tmp[x,y,c] + linear_tmp[x,y,c]) / 3
        return blury[0]
    blur_y = hcl.compute(input.shape, lambda x,y,c: blur_y(blur_x, linear_tmp, x, y, c), "blur_y", dtype = hcl.Float())
    # actualy, blur_x + blur_y is a blur 3*3.

    def LineartosRGB(linear):
        srgb = hcl.local(0, dtype = hcl.Float())
        with hcl.if_(linear <= 0.0031308):
            srgb[0] = linear * 12.92
        with hcl.else_():
            # srgb[0] = 1.055 * (linear ** (1/2.4)) - 0.055
            srgb[0] = 1.055 * hcl.power(linear, (1/2.4)) - 0.055
        return srgb[0]  
    output = hcl.compute(input.shape, lambda x,y,c: LineartosRGB(blur_y[x,y,c]), "LineartosRGB", dtype = hcl.Float())  
    # return output
    # output = hcl.compute(input.shape, lambda x,y,c: input[x,y,c], "LineartosRGB", dtype = hcl.Float())
    return output

s = hcl.create_schedule([input], linear_blur)

# s_0 = linear_blur.sRGBtoLinear
# s_1 = linear_blur.blur_x
# s_2 = linear_blur.blur_y
# s_3 = linear_blur.LineartosRGB

# s[s_0].compute_at(s[s_1], s_1.axis[1])
# s[s_1].compute_at(s[s_2], s_2.axis[1])
# s[s_2].compute_at(s[s_3], s_3.axis[1]) // didn't work, because of the boundary...




print(hcl.lower(s))


f = hcl.build(s)

print('done')

np_in = np.load("input.npy")
hcl_in = hcl.asarray(np_in, dtype = hcl.Float())
hcl_out = hcl.asarray(np.zeros(input.shape).astype('float32'), dtype = hcl.Float())
# need to specify the data type of the HeteroCL array instead of numpy array, otherwise there will be data type error

f(hcl_in, hcl_out)
print('execute done')

np_out = hcl_out.asnumpy()
np.save('output_heterocl.npy', np_out)