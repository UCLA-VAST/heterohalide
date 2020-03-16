import numpy as np 
input = np.random.randint(low = 0, high = 20, size = (648, 482)) #(w, h)
input_flat = np.transpose(input, (1, 0)).flatten()
# input_flat = input.flatten()
np.save("input.npy", input)
np.savetxt("input.txt", input_flat, fmt = "%d")