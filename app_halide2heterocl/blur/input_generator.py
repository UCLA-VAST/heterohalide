import numpy as np 
rand_img = np.random.randint(256, size = (648, 482)) #(w, h)
np.save("/curr/jiajieli/app_halide2heterocl/blur/input.npy", rand_img)
rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/app_halide2heterocl/blur/input_halide.txt', rand_img_flat, fmt = '%d')