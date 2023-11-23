import numpy as np
import time
time_ar = np.random.uniform(1.0, 10.0, size=(512, 1024))
time2_ar = np.random.uniform(1.0, 10.0, size=(512, 1024)).T

start = time.time()
for i in range(1000):
    rs = np.matmul(time_ar, time2_ar)
print(f"512*1024 matrix multiply time is: {time.time()-start}")
