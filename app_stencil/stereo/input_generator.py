import numpy as np 
left = np.random.randint(256, size = (720, 405)) #(w, h)
np.save("/curr/jiajieli/app_stencil/stereo/left.npy", left)
left_flat = np.transpose(left, (1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/app_stencil/stereo/left_halide.txt', left_flat, fmt = '%d')

right = np.random.randint(256, size = (720, 405)) #(w, h)
np.save("/curr/jiajieli/app_stencil/stereo/right.npy", right)
right_flat = np.transpose(right, (1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/app_stencil/stereo/right_halide.txt', right_flat, fmt = '%d')