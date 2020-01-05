"""
HeteroCL Tutorial : K-means Clustering Algorithm
================================================

**Author**: Yi-Hsiang Lai (seanlatias@github), Ziyan Feng

This is the K-means clustering algorithm written in Heterocl.
"""
import numpy as np
import heterocl as hcl
import time
import random
##############################################################################
# Define the number of the clustering means as K, the number of points as N,
# the number of dimensions as dim, and the number of iterations as niter
K = 16
N = 320
dim = 32
niter = 200

hcl.init()

##############################################################################
# Main Algorithm
# ==============
def top(target=None):
    points = hcl.placeholder((N, dim))
    means = hcl.placeholder((K, dim))

    def kmeans(points, means):
        def loop_kernel(labels):
            # assign cluster
            with hcl.for_(0, N, name="N") as n:
                min_dist = hcl.scalar(100000)
                with hcl.for_(0, K) as k:
                    dist = hcl.scalar(0)
                    with hcl.for_(0, dim) as d:
                        dist_ = points[n, d]-means[k, d]
                        dist.v += dist_ * dist_
                    with hcl.if_(dist.v < min_dist.v):
                        min_dist.v = dist.v
                        labels[n] = k
            # update mean
            num_k = hcl.compute((K,), lambda x: 0)
            sum_k = hcl.compute((K, dim), lambda x, y: 0)
            def calc_sum(n):
                num_k[labels[n]] += 1
                with hcl.for_(0, dim) as d:
                    sum_k[labels[n], d] += points[n, d]
            hcl.mutate((N,), lambda n: calc_sum(n), "calc_sum")
            hcl.update(means,
                    lambda k, d: sum_k[k, d]//num_k[k], "update_mean")

        labels = hcl.compute((N,), lambda x: 0)
        hcl.mutate((niter,), lambda _: loop_kernel(labels), "main_loop")
        return labels

    # create schedule and apply compute customization
    s = hcl.create_schedule([points, means], kmeans)
    main_loop = kmeans.main_loop
    update_mean = main_loop.update_mean
    s[main_loop].pipeline(main_loop.N)
    s[main_loop.calc_sum].unroll(main_loop.calc_sum.axis[0])
    fused = s[update_mean].fuse(update_mean.axis[0], update_mean.axis[1])
    s[update_mean].unroll(fused)

    print(hcl.lower(s))
    return hcl.build(s, target=target)

f = top()

points_np = np.random.randint(100, size=(N, dim))
labels_np = np.zeros(N)
means_np = points_np[random.sample(range(N), K), :]

hcl_points = hcl.asarray(points_np, dtype=hcl.Int())
hcl_means = hcl.asarray(means_np, dtype=hcl.Int())
hcl_labels = hcl.asarray(labels_np)

start = time.time()
f(hcl_points, hcl_means, hcl_labels)
total_time = time.time() - start
print("Kernel time (s): {:.2f}".format(total_time))

print("All points:")
print(hcl_points)
print("Final cluster:")
print(hcl_labels)
print("The means:")
print(hcl_means)

# heterocl IR

# produce compute0 {
#   // attr [0] extern_scope = 0
#   for (x, 0, 320) {
#     compute0[x] = 0
#   }
# }
# // attr [main_loop] storage_scope = "global"
# allocate main_loop[int32 * 1]
# produce main_loop {
#   // attr [0] extern_scope = 0
#   for (_, 0, 200) {
#     pipelined "initiation_interval"=1 (N, 0, 320) {
#       // attr [scalar0] storage_scope = "global"
#       allocate scalar0[int32 * 1]
#       produce scalar0 {
#         // attr [0] extern_scope = 0
#         scalar0[0] = 100000
#       }
#       for (i, 0, 16) {
#         // attr [scalar1] storage_scope = "global"
#         allocate scalar1[int32 * 1]
#         produce scalar1 {
#           // attr [0] extern_scope = 0
#           scalar1[0] = 0
#         }
#         for (i, 0, 32) {
#           scalar1[0] = int32((int67(scalar1[0]) + int67((int66(int33((placeholder0[(i + (N*32))] - placeholder1[(i + (i*32))])))*int66(int33((placeholder0[(i + (N*32))] - placeholder1[(i + (i*32))])))))))
#         }
#         if ((scalar1[0] < scalar0[0])) {
#           scalar0[0] = scalar1[0]
#           compute0[N] = i
#         }
#       }
#     }
#     // attr [compute1] storage_scope = "global"
#     allocate compute1[int32 * 16]
#     produce compute1 {
#       // attr [0] extern_scope = 0
#       for (x, 0, 16) {
#         compute1[x] = 0
#       }
#     }
#     // attr [compute2] storage_scope = "global"
#     allocate compute2[int32 * 16 * 32]
#     produce compute2 {
#       // attr [0] extern_scope = 0
#       for (x, 0, 16) {
#         for (y, 0, 32) {
#           compute2[(y + (x*32))] = 0
#         }
#       }
#     }
#     // attr [calc_sum] storage_scope = "global"
#     allocate calc_sum[int32 * 1]
#     produce calc_sum {
#       // attr [0] extern_scope = 0
#       unrolled "factor"=0 (n, 0, 320) {
#         compute1[compute0[n]] = (compute1[compute0[n]] + 1)
#         for (i, 0, 32) {
#           compute2[(i + (compute0[n]*32))] = int32((int33(compute2[(i + (compute0[n]*32))]) + int33(placeholder0[(i + (n*32))])))
#         }
#       }
#     }
#     // attr [update_mean] storage_scope = "global"
#     allocate update_mean[int32 * 1]
#     produce update_mean {
#       // attr [0] extern_scope = 0
#       unrolled "factor"=0 (k.d.fused, 0, 512) {
#         placeholder1[k.d.fused] = (compute2[k.d.fused]/compute1[(k.d.fused/32)])
#       }
#     }
#   }
# }
