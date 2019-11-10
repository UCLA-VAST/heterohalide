import numpy as np 
# rand_img = np.random.randint(32, size = (2448, 3264)) #(w, h)
# np.save("/curr/jiajieli/app_stencil/harris/input.npy", rand_img)
# rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
# np.savetxt('/curr/jiajieli/app_stencil/harris/input_halide.txt', rand_img_flat, fmt = '%d')

rand_img = np.random.randint(high = 16, low = 0, size = (2448, 3264)) #(w, h)
np.save("/curr/jiajieli/new/harris/input.npy", rand_img)
rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/new/harris/input.txt', rand_img_flat, fmt = '%d')