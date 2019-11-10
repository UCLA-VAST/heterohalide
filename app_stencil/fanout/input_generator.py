import numpy as np 
rand_img = np.random.randint(256, size = (200)) #(w, h)
np.save("/curr/jiajieli/app_stencil/fanout/input.npy", rand_img)
rand_img_flat = np.transpose(rand_img, (0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/app_stencil/fanout/input_halide.txt', rand_img_flat, fmt = '%d')