import heterocl as hcl
hcl.init()
final_extent_0 = 4
final_extent_1 = 4
final_extent_2 = 50
final_extent_3 = 1000
final_min_0 = 0
final_min_1 = 0
final_min_2 = 0
final_min_3 = 0
def top(input, weight_conv1, weight_conv2, weight_fc1, weight_fc2, ):
    weight_fc2_extent_0_required = (hcl.select((final_extent_0 + final_min_0) > 10, (final_extent_0 + final_min_0), 10) - hcl.select(final_min_0 < 0, final_min_0, 0))
    final_total_extent_1 = (hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_1) * hcl.cast(dtype = hcl.Int(bits = 64), expr = final_extent_0))
    conv1 = hcl.compute((3208, 12, 20, final_extent_1), lambda x, y, z, w: 0, name = "conv1", dtype = hcl.UInt(bits = 16))
    with hcl.Stage("conv1"):
        with hcl.for_(final_min_1, final_extent_1, name = "conv1_s0_n") as conv1_s0_n:
            with hcl.for_(0, 20, name = "conv1_s0_z") as conv1_s0_z:
                with hcl.for_(0, 12, name = "conv1_s0_y") as conv1_s0_y:
                    with hcl.for_(0, 3208, name = "conv1_s0_x") as conv1_s0_x:
                        conv1[conv1_s0_x, conv1_s0_y, conv1_s0_z, conv1_s0_n] = hcl.cast(dtype = hcl.UInt(bits = 16), expr = 0)
        with hcl.for_(final_min_1, final_extent_1, name = "conv1_s1_n") as conv1_s1_n:
            with hcl.for_(0, 20, name = "conv1_s1_z") as conv1_s1_z:
                with hcl.for_(0, 12, name = "conv1_s1_y") as conv1_s1_y:
                    with hcl.for_(0, 3208, name = "conv1_s1_x") as conv1_s1_x:
                        with hcl.for_(0, 5, name = "conv1_s1_r_conv1__y") as conv1_s1_r_conv1__y:
                            with hcl.for_(0, 5, name = "conv1_s1_r_conv1__x") as conv1_s1_r_conv1__x:
                                conv1[conv1_s1_x, conv1_s1_y, conv1_s1_z, conv1_s1_n] = (conv1[conv1_s1_x, conv1_s1_y, conv1_s1_z, conv1_s1_n] + (weight_conv1[conv1_s1_r_conv1__x, conv1_s1_r_conv1__y, 0, conv1_s1_z] * input[(conv1_s1_r_conv1__x + conv1_s1_x), (conv1_s1_r_conv1__y + conv1_s1_y), 0, conv1_s1_n]))
    tanh1 = hcl.compute((3208, 12, 20, final_extent_1), lambda x, y, z, w: 0, name = "tanh1", dtype = hcl.Float(bits = 32))
    with hcl.Stage("tanh1"):
        with hcl.for_(final_min_1, final_extent_1, name = "tanh1_s0_n") as tanh1_s0_n:
            with hcl.for_(0, 20, name = "tanh1_s0_z") as tanh1_s0_z:
                with hcl.for_(0, 12, name = "tanh1_s0_y") as tanh1_s0_y:
                    with hcl.for_(0, 3208, name = "tanh1_s0_x") as tanh1_s0_x:
                        tanh1[tanh1_s0_x, tanh1_s0_y, tanh1_s0_z, tanh1_s0_n] = hcl.tvm.tanh(conv1[tanh1_s0_x, tanh1_s0_y, tanh1_s0_z, tanh1_s0_n]) # manually add: hcl.tvm.tanh
    pool1_tmp = hcl.compute((3207, 11, 20, final_extent_1), lambda x, y, z, w: 0, name = "pool1_tmp", dtype = hcl.Float(bits = 32))
    with hcl.Stage("pool1_tmp"):
        with hcl.for_(final_min_1, final_extent_1, name = "pool1_tmp_s0_n") as pool1_tmp_s0_n:
            with hcl.for_(0, 20, name = "pool1_tmp_s0_z") as pool1_tmp_s0_z:
                with hcl.for_(0, 11, name = "pool1_tmp_s0_y") as pool1_tmp_s0_y:
                    with hcl.for_(0, 3207, name = "pool1_tmp_s0_x") as pool1_tmp_s0_x:
                        maximum = hcl.compute((1, 1, 1, 1), lambda x, y, z, w: 0, name = "maximum", dtype = hcl.Float(bits = 32))
                        with hcl.Stage("maximum"):
                            maximum[pool1_tmp_s0_x, pool1_tmp_s0_y, pool1_tmp_s0_z, pool1_tmp_s0_n] = hcl.cast(dtype = hcl.Float(bits = 32), expr = -100)
                            with hcl.for_(0, 2, name = "maximum_s1_r_p1__y") as maximum_s1_r_p1__y:
                                with hcl.for_(0, 2, name = "maximum_s1_r_p1__x") as maximum_s1_r_p1__x:
                                    maximum[pool1_tmp_s0_x, pool1_tmp_s0_y, pool1_tmp_s0_z, pool1_tmp_s0_n] = hcl.select(maximum[pool1_tmp_s0_x, pool1_tmp_s0_y, pool1_tmp_s0_z, pool1_tmp_s0_n] > tanh1[(maximum_s1_r_p1__x + pool1_tmp_s0_x), (maximum_s1_r_p1__y + pool1_tmp_s0_y), pool1_tmp_s0_z, pool1_tmp_s0_n], maximum[pool1_tmp_s0_x, pool1_tmp_s0_y, pool1_tmp_s0_z, pool1_tmp_s0_n], tanh1[(maximum_s1_r_p1__x + pool1_tmp_s0_x), (maximum_s1_r_p1__y + pool1_tmp_s0_y), pool1_tmp_s0_z, pool1_tmp_s0_n])
                        pool1_tmp[pool1_tmp_s0_x, pool1_tmp_s0_y, pool1_tmp_s0_z, pool1_tmp_s0_n] = maximum[pool1_tmp_s0_x, pool1_tmp_s0_y, pool1_tmp_s0_z, pool1_tmp_s0_n]
    pool1 = hcl.compute((1604, 6, 20, final_extent_1), lambda x, y, z, w: 0, name = "pool1", dtype = hcl.Float(bits = 32))
    with hcl.Stage("pool1"):
        with hcl.for_(final_min_1, final_extent_1, name = "pool1_s0_n") as pool1_s0_n:
            with hcl.for_(0, 20, name = "pool1_s0_z") as pool1_s0_z:
                with hcl.for_(0, 6, name = "pool1_s0_y") as pool1_s0_y:
                    with hcl.for_(0, 1604, name = "pool1_s0_x") as pool1_s0_x:
                        pool1[pool1_s0_x, pool1_s0_y, pool1_s0_z, pool1_s0_n] = pool1_tmp[(pool1_s0_x * 2), (pool1_s0_y * 2), pool1_s0_z, pool1_s0_n]
    conv2 = hcl.compute((1600, 2, 1, final_extent_1), lambda x, y, z, w: 0, name = "conv2", dtype = hcl.Float(bits = 32))
    with hcl.Stage("conv2"):
        with hcl.for_(final_min_1, final_extent_1, name = "conv2_s0_n") as conv2_s0_n:
            with hcl.for_(0, 2, name = "conv2_s0_y") as conv2_s0_y:
                with hcl.for_(0, 1600, name = "conv2_s0_x") as conv2_s0_x:
                    conv2[conv2_s0_x, conv2_s0_y, 0, conv2_s0_n] = hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000)
        with hcl.for_(final_min_1, final_extent_1, name = "conv2_s1_n") as conv2_s1_n:
            with hcl.for_(0, 2, name = "conv2_s1_y") as conv2_s1_y:
                with hcl.for_(0, 1600, name = "conv2_s1_x") as conv2_s1_x:
                    with hcl.for_(0, 20, name = "conv2_s1_r_conv2__z") as conv2_s1_r_conv2__z:
                        with hcl.for_(0, 5, name = "conv2_s1_r_conv2__y") as conv2_s1_r_conv2__y:
                            with hcl.for_(0, 5, name = "conv2_s1_r_conv2__x") as conv2_s1_r_conv2__x:
                                conv2[conv2_s1_x, conv2_s1_y, 0, conv2_s1_n] = (conv2[conv2_s1_x, conv2_s1_y, 0, conv2_s1_n] + (pool1[(conv2_s1_r_conv2__x + conv2_s1_x), (conv2_s1_r_conv2__y + conv2_s1_y), conv2_s1_r_conv2__z, conv2_s1_n] * hcl.cast(dtype = hcl.Float(bits = 32), expr = weight_conv2[conv2_s1_r_conv2__x, conv2_s1_r_conv2__y, conv2_s1_r_conv2__z, 0])))
    tanh2 = hcl.compute((1600, 2, 1, final_extent_1), lambda x, y, z, w: 0, name = "tanh2", dtype = hcl.Float(bits = 32))
    with hcl.Stage("tanh2"):
        with hcl.for_(final_min_1, final_extent_1, name = "tanh2_s0_n") as tanh2_s0_n:
            with hcl.for_(0, 2, name = "tanh2_s0_y") as tanh2_s0_y:
                with hcl.for_(0, 1600, name = "tanh2_s0_x") as tanh2_s0_x:
                    tanh2[tanh2_s0_x, tanh2_s0_y, 0, tanh2_s0_n] = hcl.tvm.tanh(conv2[tanh2_s0_x, tanh2_s0_y, 0, tanh2_s0_n]) # manually add: hcl.tvm.tanh
    pool2_tmp = hcl.compute((1599, 1, 1, final_extent_1), lambda x, y, z, w: 0, name = "pool2_tmp", dtype = hcl.Float(bits = 32))
    with hcl.Stage("pool2_tmp"):
        with hcl.for_(final_min_1, final_extent_1, name = "pool2_tmp_s0_n") as pool2_tmp_s0_n:
            with hcl.for_(0, 1599, name = "pool2_tmp_s0_x") as pool2_tmp_s0_x:
                maximum__1 = hcl.compute((1, 1, 1, 1), lambda x, y, z, w: 0, name = "maximum__1", dtype = hcl.Float(bits = 32))
                with hcl.Stage("maximum__1"):
                    maximum__1[pool2_tmp_s0_x, 0, 0, pool2_tmp_s0_n] = hcl.cast(dtype = hcl.Float(bits = 32), expr = -100)
                    with hcl.for_(0, 2, name = "maximum__1_s1_r_p2__y") as maximum__1_s1_r_p2__y:
                        with hcl.for_(0, 2, name = "maximum__1_s1_r_p2__x") as maximum__1_s1_r_p2__x:
                            maximum__1[pool2_tmp_s0_x, 0, 0, pool2_tmp_s0_n] = hcl.select(maximum__1[pool2_tmp_s0_x, 0, 0, pool2_tmp_s0_n] > tanh2[(maximum__1_s1_r_p2__x + pool2_tmp_s0_x), maximum__1_s1_r_p2__y, 0, pool2_tmp_s0_n], maximum__1[pool2_tmp_s0_x, 0, 0, pool2_tmp_s0_n], tanh2[(maximum__1_s1_r_p2__x + pool2_tmp_s0_x), maximum__1_s1_r_p2__y, 0, pool2_tmp_s0_n])
                pool2_tmp[pool2_tmp_s0_x, 0, 0, pool2_tmp_s0_n] = maximum__1[pool2_tmp_s0_x, 0, 0, pool2_tmp_s0_n]
    pool2 = hcl.compute((800, 1, 1, final_extent_1), lambda x, y, z, w: 0, name = "pool2", dtype = hcl.Float(bits = 32))
    with hcl.Stage("pool2"):
        with hcl.for_(final_min_1, final_extent_1, name = "pool2_s0_n") as pool2_s0_n:
            with hcl.for_(0, 800, name = "pool2_s0_x") as pool2_s0_x:
                pool2[pool2_s0_x, 0, 0, pool2_s0_n] = pool2_tmp[(pool2_s0_x * 2), 0, 0, pool2_s0_n]
    fc1 = hcl.compute((500, final_extent_1), lambda x, y: 0, name = "fc1", dtype = hcl.Float(bits = 32))
    with hcl.Stage("fc1"):
        with hcl.for_(final_min_1, final_extent_1, name = "fc1_s0_n") as fc1_s0_n:
            with hcl.for_(0, 500, name = "fc1_s0_x") as fc1_s0_x:
                fc1[fc1_s0_x, fc1_s0_n] = hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000)
        with hcl.for_(final_min_1, final_extent_1, name = "fc1_s1_n") as fc1_s1_n:
            with hcl.for_(0, 500, name = "fc1_s1_x") as fc1_s1_x:
                with hcl.for_(0, 800, name = "fc1_s1_r_fc1__x") as fc1_s1_r_fc1__x:
                    fc1[fc1_s1_x, fc1_s1_n] = (fc1[fc1_s1_x, fc1_s1_n] + (pool2[fc1_s1_r_fc1__x, 0, 0, fc1_s1_n] * hcl.cast(dtype = hcl.Float(bits = 32), expr = weight_fc1[fc1_s1_x, fc1_s1_r_fc1__x])))
    tanh3 = hcl.compute((500, final_extent_1), lambda x, y: 0, name = "tanh3", dtype = hcl.Float(bits = 32))
    with hcl.Stage("tanh3"):
        with hcl.for_(final_min_1, final_extent_1, name = "tanh3_s0_y") as tanh3_s0_y:
            with hcl.for_(0, 500, name = "tanh3_s0_x") as tanh3_s0_x:
                tanh3[tanh3_s0_x, tanh3_s0_y] = hcl.tvm.tanh(fc1[tanh2_s0_x, tanh2_s0_y, 0, tanh2_s0_n]) # manually add: hcl.tvm.tanh
    fc2 = hcl.compute((weight_fc2_extent_0_required, final_extent_1), lambda x, y: 0, name = "fc2", dtype = hcl.Float(bits = 32))
    with hcl.Stage("fc2"):
        with hcl.for_(final_min_1, final_extent_1, name = "fc2_s0_n") as fc2_s0_n:
            with hcl.for_(hcl.select(final_min_0 < 0, final_min_0, 0), weight_fc2_extent_0_required, name = "fc2_s0_x") as fc2_s0_x:
                fc2[fc2_s0_x, fc2_s0_n] = hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000)
        with hcl.for_(final_min_1, final_extent_1, name = "fc2_s1_n") as fc2_s1_n:
            with hcl.for_(hcl.select(final_min_0 < 0, final_min_0, 0), weight_fc2_extent_0_required, name = "fc2_s1_x") as fc2_s1_x:
                with hcl.for_(0, 500, name = "fc2_s1_r_fc2__x") as fc2_s1_r_fc2__x:
                    fc2[fc2_s1_x, fc2_s1_n] = (fc2[fc2_s1_x, fc2_s1_n] + (tanh3[fc2_s1_r_fc2__x, fc2_s1_n] * hcl.cast(dtype = hcl.Float(bits = 32), expr = weight_fc2[fc2_s1_x, fc2_s1_r_fc2__x])))
    final = hcl.compute((4, 4, 50, 1000), lambda x, y, z, w: 0, name = "final", dtype = hcl.Float(bits = 32))
    with hcl.Stage("final"):
        with hcl.for_(final_min_1, final_extent_1, name = "final_s0_n") as final_s0_n:
            with hcl.for_(final_min_0, final_extent_0, name = "final_s0_x") as final_s0_x:
                sum = hcl.compute((1), lambda x: 0, name = "sum", dtype = hcl.Float(bits = 32))
                with hcl.Stage("sum"):
                    sum[final_s0_n] = hcl.cast(dtype = hcl.Float(bits = 32), expr = 0.000000)
                    with hcl.for_(0, 10, name = "sum_s1_r_softmax__x") as sum_s1_r_softmax__x:
                        sum[final_s0_n] = (sum[final_s0_n] + hcl.call_pure_intrin("float", "pow", 2.718280, fc2[sum_s1_r_softmax__x, final_s0_n]))
                final[final_s0_x, final_s0_n] = (hcl.call_pure_intrin("float", "pow", 2.718280, fc2[final_s0_x, final_s0_n])//sum[final_s0_n])
    return final
input = hcl.placeholder((28, 28, 1, 4, ), name = "input", dtype = hcl.UInt(bits = 16))
weight_conv1 = hcl.placeholder((5, 5, 1, 20, ), name = "weight_conv1", dtype = hcl.UInt(bits = 16))
weight_conv2 = hcl.placeholder((5, 5, 20, 50, ), name = "weight_conv2", dtype = hcl.UInt(bits = 16))
weight_fc1 = hcl.placeholder((500, 800, ), name = "weight_fc1", dtype = hcl.UInt(bits = 16))
weight_fc2 = hcl.placeholder((10, 500, ), name = "weight_fc2", dtype = hcl.UInt(bits = 16))
s = hcl.create_schedule([input, weight_conv1, weight_conv2, weight_fc1, weight_fc2, ], top)
f = hcl.build(s)
print(hcl.lower(s))
# import numpy as np
# np_input = np.load("input.npy")
# hcl_input = hcl.asarray(np_input, dtype = hcl.UInt(bits = 16))
# np_weight_conv1 = np.load("weight_conv1.npy")
# hcl_weight_conv1 = hcl.asarray(np_weight_conv1, dtype = hcl.UInt(bits = 16))
# np_weight_conv2 = np.load("weight_conv2.npy")
# hcl_weight_conv2 = hcl.asarray(np_weight_conv2, dtype = hcl.UInt(bits = 16))
# np_weight_fc1 = np.load("weight_fc1.npy")
# hcl_weight_fc1 = hcl.asarray(np_weight_fc1, dtype = hcl.UInt(bits = 16))
# np_weight_fc2 = np.load("weight_fc2.npy")
# hcl_weight_fc2 = hcl.asarray(np_weight_fc2, dtype = hcl.UInt(bits = 16))
# output_shape = (4, 4, 50, 1000, )
# hcl_out = hcl.asarray(np.zeros(output_shape), dtype = hcl.Float(bits = 32))
# f(hcl_input, hcl_weight_conv1, hcl_weight_conv2, hcl_weight_fc1, hcl_weight_fc2, hcl_out)
# np_out = hcl_out.asnumpy()
# np.save("output_heterocl.npy", np_out)
# print(hcl.build(s, target = "soda"))
