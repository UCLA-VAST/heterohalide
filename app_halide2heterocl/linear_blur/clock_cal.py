import numpy as np 
clock = np.loadtxt("clock.txt")
avg_clock = np.sum(clock) / np.size(clock)
print(avg_clock)