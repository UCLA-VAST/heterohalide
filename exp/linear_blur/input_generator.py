import numpy as np 
input = 100 * np.random.random(size = (768, 1280, 3)) #(w, h, c)
input_flat = np.transpose(input, (2, 1, 0)).flatten()

np.save("input.npy", input)
np.savetxt("input.txt", input_flat, fmt = "%f")