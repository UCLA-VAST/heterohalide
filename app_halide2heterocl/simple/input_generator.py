import numpy as np 
input = np.random.randint(0, 10, size = (10, 10)) #[w, h]
input_flat = np.transpose(input, (1,0)).flatten()

np.save("input.npy", input)
np.savetxt("input.txt", input_flat, fmt = "%d")