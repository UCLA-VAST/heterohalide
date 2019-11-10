import numpy as np 
rand_img = np.random.randint(256, size = (12, 12)) #(w, h)
np.save("/curr/jiajieli/app_stencil/gaussian/input.npy", rand_img)
rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/app_stencil/gaussian/input_halide.txt', rand_img_flat, fmt = '%d')