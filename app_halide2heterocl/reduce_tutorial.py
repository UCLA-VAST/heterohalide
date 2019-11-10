import heterocl as hcl 
# example 1.1 - basic reduction : summation
my_sum = hcl.reducer(0, lambda x, y: x+y)
A = hcl.placeholder((10,))
r = hcl.reduce_axis(0, 10)
def reducesum(A):
    B = hcl.compute((1,), lambda x: my_sum(A[r], axis=r))
    return B

s = hcl.create_schedule([A],reducesum)
f = hcl.build(s)
print(hcl.lower(s))

import numpy as np 
A = np.array([1,2,3,4,5,6,7,8,9,10])
hcl_A = hcl.asarray(A)
hcl_B = hcl.asarray(np.zeros([1]))
f(hcl_A,hcl_B)

A_out = hcl_A.asnumpy()
B_out = hcl_B.asnumpy()
print(A_out)
print(B_out)

# # equivalent code
# B[0] = 0
# for r in (0, 10):
#     B[0] = A[r] + B[0]

# # example 1.2 - with condition
# B = hcl.compute((1,), lambda x: my_sum(A[r], axis=r, where=A[r]>5))

# # equivalent code
# B[0] = 0
# for r in (0, 10):
#     if A[r] > 5:
#         B[0] = A[r] + B[0]

# # example 1.3 - with data type specification
# B = hcl.compute((1,), lambda x: my_sum(A[r], axis=r, dtype=hcl.UInt(4)))
# # the output will be downsize to UInt(4)

# # example 2 = a more complicated reduction
# # x is the input, y is the accumulator
# def my_reduction(x, y):
#     with hcl.if_(x > 5):
#         hcl.return_(y + x)
#     with hcl.else_():
#         hcl.return_(y - x)
# my_sum = hcl.reducer(0, my_reduction)
# A = hcl.placeholder((10,))
# r = hcl.reduce_axis(0, 10)
# B = hcl.compute((1,), lambda x: my_sum(A[r], axis=r))

# # equivalent code
# B[0] = 0
# for r in range(0, 10):
#     if A[r] > 5:
#         B[0] = B[0] + A[r]
#     else:
#         B[0] = B[0] - A[r]

# # example 3 - multiple reduce axes
# A = hcl.placeholder((10, 10))
# r1 = hcl.reduce_axis(0, 10)
# r2 = hcl.reduce_axis(0, 10)
# B = hcl.compute((1,), lambda x: my_sum(A[r1, r2], axis=[r1, r2]))

# # equivalent code
# B[0] = 0
# for r1 in (0, 10):
#     for r2 in (0, 10):
#         B[0] = A[r1, r2] + B[0]

# # example 4 - write a sorting algorithm with reduction
# init = hcl.compute((10,), lambda x: 11)
# def freduce(x, Y):
#     with hcl.for_(0, 10) as i:
#         with hcl.if_(x < Y[i]):
#             with hcl.for_(9, i, -1) as j:
#                 Y[j] = Y[j-1]
#             Y[i] = x
#             hcl.break_()
# my_sort = hcl.reducer(init, freduce)
# A = hcl.placeholder((10, 10))
# r = hcl.reduce_axis(0, 10)
# # note that we need to use the underscore the mark the reduction axis
# B = hcl.compute(A.shape, lambda _x, y: my_sort(A[r, y], axis=r))