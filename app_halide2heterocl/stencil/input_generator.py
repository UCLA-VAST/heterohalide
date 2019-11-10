import numpy as np 
input = np.random.randint(low = 0, high = 2, size = (1536, 2560), dtype = 'uint16') # [w, h]
input_flat = np.transpose(input, (1, 0)).flatten()  # h = 2560, w = 1536

np.save("input.npy", input)
np.savetxt("/curr/jiajieli/app_halide2heterocl/stencil/input_halide.txt", input_flat, fmt = '%d')


# input = np.random.randint(low = 0, high = 200, size = (2560, 1536)) # just need one channel of image

# np.save("input_.npy", input)
# input_flat = np.transpose(input, (1,0)).flatten() #(1536, 2560): h, w
# np.savetxt("/curr/jiajieli/app_halide2heterocl/stencil/input_halide_.txt", input_flat, fmt = '%d')

