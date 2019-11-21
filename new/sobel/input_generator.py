import numpy as np 
input = np.random.randint(low = 0, high = 5, size = (642, 482)) #(w, h)
input_flat = np.transpose(input, (1, 0)).flatten()
np.save("input.npy", input)
np.savetxt("input.txt", input_flat, fmt = "%d")

filter_x = np.array([[-1, 0, 1], [-3, 0, 3], [-1, 0, 1]])
filter_x_flat = np.transpose(filter_x, (1, 0)).flatten()
np.save("filter_x.npy", filter_x)
np.savetxt("filter_x.txt", filter_x_flat, fmt = "%d")

filter_y = np.array([[-1, -3, -1], [0, 0, 0], [1, 3, 1]])
filter_y_flat = np.transpose(filter_y, (1, 0)).flatten()
np.save("filter_y.npy", filter_y)
np.savetxt("filter_y.txt", filter_y_flat, fmt = "%d")
