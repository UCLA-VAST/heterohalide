"""
HeteroCL Tutorial : LeNet Inference
===================================

**Author**: Yi-Hsiang Lai (seanlatias@github)

Build the LeNet inference model by using hlib. In this tutorial, we demonstrate
how we can apply inference-time quantization to an existing model.
"""
import heterocl as hcl
import hlib
import numpy as np

###############################################################################
# Initially we let every operation to be a floating-point operation
hcl.init(hcl.Float())

###############################################################################
# You can define your own layer without using the one provided in hlib
def softmax(out, x):
    assert len(x.shape) == 2, "only support 2-dim softmax"
    m, n = x.shape
    k = hcl.reduce_axis(0, n)
    max_elem = hcl.compute((m,), lambda i: hcl.max(x[i, k], axis=k))
    k = hcl.reduce_axis(0, n)
    expsum = hcl.compute((m,),
            lambda i: hcl.sum(hcl.exp(x[i, k] - max_elem[i]), axis=k))
    return hcl.update(out,
            lambda i, j: hcl.exp(x[i, j] - max_elem[i]) / expsum[i])

###############################################################################
# The main function for build the LeNet inference model
def build_lenet(input_image, weight_conv1, weight_conv2,
                weight_fc1, weight_fc2, lenet):
    # first conv
    conv1 = hlib.nn.conv2d_nchw(input_image, weight_conv1)
    tanh1 = hlib.nn.tanh(conv1, "tanh1")
    pool1 = hlib.nn.max_pool(tanh1, kernel=(2,2), stride=(2,2))
    # second conv
    conv2 = hlib.nn.conv2d_nchw(pool1, weight_conv2)
    tanh2 = hlib.nn.tanh(conv2, "tanh2")
    pool2 = hlib.nn.max_pool(tanh2, kernel=(2,2), stride=(2,2))
    # first fc
    flat = hlib.nn.flatten(pool2)
    fc1 = hlib.nn.dense(flat, weight_fc1)
    tanh3 = hlib.nn.tanh(fc1, "tanh3")
    # second fc
    fc2 =  hlib.nn.dense(tanh3, weight_fc2)
    # loss
    return softmax(lenet, fc2)

###############################################################################
# Download the dataset from mxnet
# import mxnet as mx
# # download pretrained lenet model
# mx.gluon.utils.download('https://gist.githubusercontent.com/Huyuwei/dc00ce83f537914c64a204133d23b019/raw/79af41e7c8ba9120ea7f35fb1d0484b65bccd54f/lenet-0010.params')
# mx.gluon.utils.download('https://gist.githubusercontent.com/Huyuwei/dc00ce83f537914c64a204133d23b019/raw/79af41e7c8ba9120ea7f35fb1d0484b65bccd54f/lenet-symbol.json')
# sym, arg_params, aux_params = mx.model.load_checkpoint('lenet', 10)
# # get weights
# weight_conv1_np = arg_params['convolution0_weight'].asnumpy()
# weight_conv2_np = arg_params['convolution1_weight'].asnumpy()
# weight_fc1_np = arg_params['fullyconnected0_weight'].asnumpy()
# weight_fc2_np = arg_params['fullyconnected1_weight'].asnumpy()

# ###############################################################################
# # Define the quantized data type and run the inference
qtype1 = hcl.Fixed(16, 14)
qtype2 = hcl.Fixed(16, 14)
# correct_sum = 0
batch_size = 1000
# mnist = mx.test_utils.get_mnist()

###############################################################################
# In this example, we quantize the weights to `qtype1` and the activations to
# `qtype2`. To quantize the placeholders, simply specify the `dtype` field. For
# the internal tensors, we use `hcl.quantize` API.
def build_lenet_inf(batch_size=batch_size, target=None):
    # set up input/output placeholders
    input_image = hcl.placeholder((batch_size, 1, 28, 28), "input_image")
    weight_conv1 = hcl.placeholder((20, 1, 5, 5), "weight_conv1", qtype1)
    weight_conv2 = hcl.placeholder((50, 20, 5, 5), "weight_conv2", qtype1)
    weight_fc1 = hcl.placeholder((500, 800), "weight_fc1", qtype1)
    weight_fc2 = hcl.placeholder((10, 500), "weight_fc2", qtype1)
    lenet = hcl.placeholder((batch_size, 10), "lenet")
    # create a quantization scheme
    scheme = hcl.create_scheme(
            [input_image, weight_conv1, weight_conv2,
             weight_fc1, weight_fc2, lenet], build_lenet)
    # quantize the three activation layers
    scheme.quantize(
            [build_lenet.tanh1, build_lenet.tanh2, build_lenet.tanh3], qtype2)
    s = hcl.create_schedule_from_scheme(scheme)

    print(hcl.lower(s))
    return hcl.build(s, target=target)

f = build_lenet_inf()

###############################################################################
# Prepare the numpy arrays for testing. Remember that we need to set the input
# tensors with the same type as the placeholders
# weight_conv1_hcl = hcl.asarray(weight_conv1_np, dtype=qtype1)
# weight_conv2_hcl = hcl.asarray(weight_conv2_np, dtype=qtype1)
# weight_fc1_hcl = hcl.asarray(weight_fc1_np, dtype=qtype1)
# weight_fc2_hcl = hcl.asarray(weight_fc2_np, dtype=qtype1)

# for i in range(10000 // batch_size):
#     label = mnist['test_label'][i*batch_size:(i+1)*batch_size]
#     input_image_np = mnist['test_data'][i*batch_size:(i+1)*batch_size]
#     input_image_hcl = hcl.asarray(input_image_np)
#     output_hcl = hcl.asarray(np.zeros((batch_size,10)))
#     f(input_image_hcl, weight_conv1_hcl, weight_conv2_hcl,
#             weight_fc1_hcl, weight_fc2_hcl, output_hcl)
#     prediction = np.argmax(output_hcl.asnumpy(), axis=1)
#     correct_sum += np.sum(np.equal(prediction, label))

# ###############################################################################
# # Print the result

# print("Testing accuracy: {}".format(correct_sum / 10000.))

# remove downloaded files
# import os
# os.remove("t10k-images-idx3-ubyte.gz")
# os.remove("t10k-labels-idx1-ubyte.gz")
# os.remove("train-images-idx3-ubyte.gz")
# os.remove("train-labels-idx1-ubyte.gz")
# os.remove("lenet-0010.params")
# os.remove("lenet-symbol.json")

# assert correct_sum == 9882





# HeteroCL IR
# // attr [conv2d] storage_scope = "global"
# allocate conv2d[float32 * 1000 * 20 * 24 * 24]
# produce conv2d {
#   // attr [0] extern_scope = 0
#   for "p"=5 "q"=5 "in_num"=1 "out_num"=20 "out_img_w"=24 "out_img_h"=24 "cin_dtype"="float32" "filter_dtype"="fixed16_14" "app_name"="cnn" (nn, 0, 1000) {
#     for (ff, 0, 20) {
#       for (yy, 0, 24) {
#         for (xx, 0, 24) {
#           // attr [reducer6] storage_scope = "global"
#           allocate reducer6[float32 * 1]
#           produce reducer6 {
#             // attr [0] extern_scope = 0
#             reducer6[0] = 0.000000f
#           }
#           for (ra15, 0, 5) {
#             for (ra16, 0, 5) {
#               reducer6[0] = ((input_image[(((xx + ra16) + ((yy + ra15)*28)) + (nn*784))]*float32(weight_conv1[((ra16 + (ra15*5)) + (ff*25))])) + reducer6[0])
#             }
#           }
#           conv2d[(((xx + (yy*24)) + (ff*576)) + (nn*11520))] = reducer6[0]
#         }
#       }
#     }
#   }
# }
# // attr [tanh1] storage_scope = "global"
# allocate tanh1[fixed16_14 * 1000 * 20 * 24 * 24]
# produce tanh1 {
#   // attr [0] extern_scope = 0
#   for "app_name"="tanh" (args, 0, 1000) {
#     for (args0, 0, 20) {
#       for (args1, 0, 24) {
#         for (args2, 0, 24) {
#           tanh1[(((args2 + (args1*24)) + (args0*576)) + (args*11520))] = fixed16_14(tanh(float64(conv2d[(((args2 + (args1*24)) + (args0*576)) + (args*11520))])))
#         }
#       }
#     }
#   }
# }
# // attr [max_pool] storage_scope = "global"
# allocate max_pool[float32 * 1000 * 20 * 12 * 12]
# produce max_pool {
#   // attr [0] extern_scope = 0
#   for "out_img_w"=12 "out_img_h"=12 "in_num"=20 "kernel_h"=2 "kernel_w"=2 "stride_h"=2 "stride_w"=2 "app_name"="max_pool" (i, 0, 1000) {
#     for (c, 0, 20) {
#       for (h, 0, 12) {
#         for (w, 0, 12) {
#           // attr [reducer7] storage_scope = "global"
#           allocate reducer7[float32 * 1]
#           produce reducer7 {
#             // attr [0] extern_scope = 0
#             reducer7[0] = -1.000000f
#           }
#           for (ra17, 0, 2) {
#             for (ra18, 0, 2) {
#               reducer7[0] = max(float32(tanh1[(((((w*2) + ra18) + (((h*2) + ra17)*24)) + (c*576)) + (i*11520))]), reducer7[0])
#             }
#           }
#           max_pool[(((w + (h*12)) + (c*144)) + (i*2880))] = reducer7[0]
#         }
#       }
#     }
#   }
# }
# // attr [conv2d] storage_scope = "global"
# allocate conv2d[float32 * 1000 * 50 * 8 * 8]
# produce conv2d {
#   // attr [0] extern_scope = 0
#   for "p"=5 "q"=5 "in_num"=20 "out_num"=50 "out_img_w"=8 "out_img_h"=8 "cin_dtype"="float32" "filter_dtype"="fixed16_14" "app_name"="cnn" (nn, 0, 1000) {
#     for (ff, 0, 50) {
#       for (yy, 0, 8) {
#         for (xx, 0, 8) {
#           // attr [reducer8] storage_scope = "global"
#           allocate reducer8[float32 * 1]
#           produce reducer8 {
#             // attr [0] extern_scope = 0
#             reducer8[0] = 0.000000f
#           }
#           for (ra19, 0, 20) {
#             for (ra20, 0, 5) {
#               for (ra21, 0, 5) {
#                 reducer8[0] = ((max_pool[((((xx + ra21) + ((yy + ra20)*12)) + (ra19*144)) + (nn*2880))]*float32(weight_conv2[(((ra21 + (ra20*5)) + (ra19*25)) + (ff*500))])) + reducer8[0])
#               }
#             }
#           }
#           conv2d[(((xx + (yy*8)) + (ff*64)) + (nn*3200))] = reducer8[0]
#         }
#       }
#     }
#   }
# }
# // attr [tanh2] storage_scope = "global"
# allocate tanh2[fixed16_14 * 1000 * 50 * 8 * 8]
# produce tanh2 {
#   // attr [0] extern_scope = 0
#   for "app_name"="tanh" (args, 0, 1000) {
#     for (args0, 0, 50) {
#       for (args1, 0, 8) {
#         for (args2, 0, 8) {
#           tanh2[(((args2 + (args1*8)) + (args0*64)) + (args*3200))] = fixed16_14(tanh(float64(conv2d[(((args2 + (args1*8)) + (args0*64)) + (args*3200))])))
#         }
#       }
#     }
#   }
# }
# // attr [max_pool] storage_scope = "global"
# allocate max_pool[float32 * 1000 * 50 * 4 * 4]
# produce max_pool {
#   // attr [0] extern_scope = 0
#   for "out_img_w"=4 "out_img_h"=4 "in_num"=50 "kernel_h"=2 "kernel_w"=2 "stride_h"=2 "stride_w"=2 "app_name"="max_pool" (i, 0, 1000) {
#     for (c, 0, 50) {
#       for (h, 0, 4) {
#         for (w, 0, 4) {
#           // attr [reducer9] storage_scope = "global"
#           allocate reducer9[float32 * 1]
#           produce reducer9 {
#             // attr [0] extern_scope = 0
#             reducer9[0] = -1.000000f
#           }
#           for (ra22, 0, 2) {
#             for (ra23, 0, 2) {
#               reducer9[0] = max(float32(tanh2[(((((w*2) + ra23) + (((h*2) + ra22)*8)) + (c*64)) + (i*3200))]), reducer9[0])
#             }
#           }
#           max_pool[(((w + (h*4)) + (c*16)) + (i*800))] = reducer9[0]
#         }
#       }
#     }
#   }
# }
# // attr [compute3] storage_scope = "global"
# allocate compute3[float32 * 1000 * 800]
# produce compute3 {
#   // attr [0] extern_scope = 0
#   for "app_name"="flatten" (i, 0, 1000) {
#     for (j, 0, 800) {
#       compute3[(j + (i*800))] = max_pool[((((((j/4) % 4)*4) + (j % 4)) + ((j/16)*16)) + (i*800))]
#     }
#   }
# }
# // attr [dense] storage_scope = "global"
# allocate dense[float32 * 1000 * 500]
# produce dense {
#   // attr [0] extern_scope = 0
#   for "k"=800 "j"=500 "i"=1000 "app_name"="mm" (i, 0, 1000) {
#     for (j, 0, 500) {
#       // attr [reducer10] storage_scope = "global"
#       allocate reducer10[float32 * 1]
#       produce reducer10 {
#         // attr [0] extern_scope = 0
#         reducer10[0] = 0.000000f
#       }
#       for (ra24, 0, 800) {
#         reducer10[0] = ((compute3[(ra24 + (i*800))]*float32(weight_fc1[(ra24 + (j*800))])) + reducer10[0])
#       }
#       dense[(j + (i*500))] = reducer10[0]
#     }
#   }
# }
# // attr [tanh3] storage_scope = "global"
# allocate tanh3[fixed16_14 * 1000 * 500]
# produce tanh3 {
#   // attr [0] extern_scope = 0
#   for "app_name"="tanh" (args, 0, 1000) {
#     for (args0, 0, 500) {
#       tanh3[(args0 + (args*500))] = fixed16_14(tanh(float64(dense[(args0 + (args*500))])))
#     }
#   }
# }
# // attr [dense] storage_scope = "global"
# allocate dense[float32 * 1000 * 10]
# produce dense {
#   // attr [0] extern_scope = 0
#   for "k"=500 "j"=10 "i"=1000 "app_name"="mm" (i, 0, 1000) {
#     for (j, 0, 10) {
#       // attr [reducer11] storage_scope = "global"
#       allocate reducer11[float32 * 1]
#       produce reducer11 {
#         // attr [0] extern_scope = 0
#         reducer11[0] = 0.000000f
#       }
#       for (ra25, 0, 500) {
#         reducer11[0] = (float32((fixed32_14(tanh3[(ra25 + (i*500))])*fixed32_14(weight_fc2[(ra25 + (j*500))]))) + reducer11[0])
#       }
#       dense[(j + (i*10))] = reducer11[0]
#     }
#   }
# }
# // attr [compute4] storage_scope = "global"
# allocate compute4[float32 * 1000]
# produce compute4 {
#   // attr [0] extern_scope = 0
#   for (i, 0, 1000) {
#     // attr [max] storage_scope = "global"
#     allocate max[int32 * 1]
#     produce max {
#       // attr [0] extern_scope = 0
#       max[0] = 0
#     }
#     for (ra26, 0, 10) {
#       max[0] = int32(max(dense[(ra26 + (i*10))], float32(max[0])))
#     }
#     compute4[i] = float32(max[0])
#   }
# }
# // attr [compute5] storage_scope = "global"
# allocate compute5[float32 * 1000]
# produce compute5 {
#   // attr [0] extern_scope = 0
#   for (i, 0, 1000) {
#     // attr [sum] storage_scope = "global"
#     allocate sum[int32 * 1]
#     produce sum {
#       // attr [0] extern_scope = 0
#       sum[0] = 0
#     }
#     for (ra27, 0, 10) {
#       sum[0] = int32((exp(float64((dense[(ra27 + (i*10))] - compute4[i]))) + float64(sum[0])))
#     }
#     compute5[i] = float32(sum[0])
#   }
# }
# // attr [update1] storage_scope = "global"
# allocate update1[float32 * 1]
# produce update1 {
#   // attr [0] extern_scope = 0
#   for (i, 0, 1000) {
#     for (j, 0, 10) {
#       lenet[(j + (i*10))] = float32((exp(float64((dense[(j + (i*10))] - compute4[i])))/float64(compute5[i])))
#     }
#   }
# }