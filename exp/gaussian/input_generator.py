import numpy as np 
rand_img = np.random.randint(256, size = (400, 600)) #(w, h)
np.save("/curr/jiajieli/new/gaussian/input.npy", rand_img)
rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/new/gaussian/input_halide.txt', rand_img_flat, fmt = '%d')