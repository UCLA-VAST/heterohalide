import numpy as np 
input = np.random.randint(low = -100, high = 100, size = (67, 67, 32, 4)) # (w, h, c, n)
input_flat = np.transpose(input, (3,2,1,0)).flatten() # will flatten in this form: (67,67,32,4)... will flatten the inner part at first
filter = np.random.randint(low = -20, high = 20, size = (3, 3, 32, 32)) # (w, h, c, n)
filter_flat = np.transpose(filter, (3,2,1,0)).flatten()
bias = np.random.randint(low = -50, high = 50, size = (32,)) #c

np.save("input.npy", input)
np.save("filter.npy", filter)
np.save("bias.npy", bias)
np.savetxt('/curr/jiajieli/new/conv/input.txt', input_flat, fmt = '%d')
np.savetxt('/curr/jiajieli/new/conv/filter.txt', filter_flat, fmt = '%d')
np.savetxt('/curr/jiajieli/new/conv/bias.txt', bias, fmt = '%d')