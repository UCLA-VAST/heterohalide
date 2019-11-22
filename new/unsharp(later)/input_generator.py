import numpy as np 
rand_img = np.random.randint(256, size = (16, 16, 3)) #(w, h)
np.save("/curr/jiajieli/app_stencil/unsharp/input.npy", rand_img)
rand_img_flat = np.transpose(rand_img, (2, 1, 0)).flatten() # to first flaten dim w
np.savetxt('/curr/jiajieli/app_stencil/unsharp/input_halide.txt', rand_img_flat, fmt = '%d')

# input_0 = rand_img[:,:,0]
# input_1 = rand_img[:,:,1]
# input_2 = rand_img[:,:,2]
# np.save("/curr/jiajieli/app_stencil/unsharp/input_0.npy", input_0)
# np.save("/curr/jiajieli/app_stencil/unsharp/input_1.npy", input_1)
# np.save("/curr/jiajieli/app_stencil/unsharp/input_2.npy", input_2)


# rand_img = np.random.randint(low = -100, high = 100, size = (4, 4)) #(w, h)
# np.save("/curr/jiajieli/app_stencil/unsharp/input_test.npy", rand_img)
# rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
# np.savetxt('/curr/jiajieli/app_stencil/unsharp/input_test_halide.txt', rand_img_flat, fmt = '%d')

# rand_img = np.random.randint(100, size = (4, 4)) #(w, h)
# np.save("/curr/jiajieli/app_stencil/unsharp/input_test_1.npy", rand_img)
# rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
# np.savetxt('/curr/jiajieli/app_stencil/unsharp/input_test_1_halide.txt', rand_img_flat, fmt = '%d')

# rand_img = np.random.randint(100, size = (4, 4)) #(w, h)
# np.save("/curr/jiajieli/app_stencil/unsharp/input_test_2.npy", rand_img)
# rand_img_flat = np.transpose(rand_img, (1, 0)).flatten() # to first flaten dim w
# np.savetxt('/curr/jiajieli/app_stencil/unsharp/input_test_2_halide.txt', rand_img_flat, fmt = '%d')