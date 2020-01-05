#include <Halide.h>
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    Buffer<uint16_t> input(28, 28, 1, 4); // width, height, channels, batch
    Buffer<uint16_t> weight_conv1(5, 5, 1, 20); // kernel_w, kernel_h, input_channel, output_channel
    Buffer<uint16_t> weight_conv2(5, 5, 20, 50); 
    Buffer<uint16_t> weight_fc1(500, 800);
    Buffer<uint16_t> weight_fc2(10, 500);

    // initialization
    for (int c = 0; c < input.dim(3).extent(); c++) {
        for (int z = 0; z < input.channels(); z++) {
            for (int y = 0; y < input.height(); y++) {
                for (int x = 0; x < input.width(); x++) {
                    input(x, y, z, c) = rand() % 10;
                }
            }
        }
    }

    for (int c = 0; c < weight_conv1.dim(3).extent(); c++) {
        for (int z = 0; z < weight_conv1.channels(); z++) {
            for (int y = 0; y < weight_conv1.height(); y++) {
                for (int x = 0; x < weight_conv1.width(); x++) {
                    weight_conv1(x, y, z, c) = rand() % 3;
                }
            }
        }
    }

    for (int c = 0; c < weight_conv2.dim(3).extent(); c++) {
        for (int z = 0; z < weight_conv2.channels(); z++) {
            for (int y = 0; y < weight_conv2.height(); y++) {
                for (int x = 0; x < weight_conv2.width(); x++) {
                    weight_conv2(x, y, z, c) = rand() % 3;
                }
            }
        }
    }        

    for (int y = 0; y < weight_fc1.height(); y++) {
        for (int x = 0; x < weight_fc1.width(); x++) {
            weight_fc1(x, y) = rand() % 3;
        }
    }
    
    for (int y = 0; y < weight_fc2.height(); y++) {
        for (int x = 0; x < weight_fc2.width(); x++) {
            weight_fc2(x, y) = rand() % 3;
        }
    }

    Var x("x"), y("y"), z("z"), n("n");

    // first conv
    Func conv1("conv1");
    RDom r_conv1(weight_conv1.dim(0).min(), weight_conv1.dim(0).extent(),
        weight_conv1.dim(1).min(), weight_conv1.dim(1).extent(),
        weight_conv1.dim(2).min(), weight_conv1.dim(2).extent());
    conv1(x, y, z, n) += weight_conv1(r_conv1.x, r_conv1.y, r_conv1.z, z) * input(x + r_conv1.x, y + r_conv1.y, r_conv1.z, n);

    Func tanh1("tanh1");
    tanh1(x, y, z, n) = tanh(conv1(x, y, z, n));

    Func pool1_tmp("pool1_tmp");
    RDom r_p1(0, 2, 0, 2); // pooling kernel (2, 2)
    pool1_tmp(x, y, z, n) = maximum(tanh1(x + r_p1.x, y + r_p1.y, z, n));

    Func pool1("pool1");
    int p1_stride_x = 2; int p1_stride_y = 2;
    pool1(x, y, z, n) = pool1_tmp(x * p1_stride_x, y * p1_stride_y, z, n);

    // schedule
    conv1.compute_root();
    tanh1.compute_root();
    pool1_tmp.compute_root();
    pool1.compute_root();

    // second conv
    Func conv2("conv2");
    RDom r_conv2(weight_conv2.dim(0).min(), weight_conv2.dim(0).extent(),
        weight_conv2.dim(1).min(), weight_conv2.dim(1).extent(),
        weight_conv2.dim(2).min(), weight_conv2.dim(2).extent());
    conv2(x, y, z, n) += weight_conv2(r_conv2.x, r_conv2.y, r_conv2.z, z) * pool1(x + r_conv2.x, y + r_conv2.y, r_conv2.z, n);

    Func tanh2("tanh2");
    tanh2(x, y, z, n) = tanh(conv2(x, y, z, n));

    Func pool2_tmp("pool2_tmp");
    RDom r_p2(0, 2, 0, 2); // pooling kernel (2, 2)
    pool2_tmp(x, y, z, n) = maximum(tanh2(x + r_p2.x, y + r_p2.y, z, n));

    Func pool2("pool2");
    int p2_stride_x = 2; int p2_stride_y = 2;
    pool2(x, y, z, n) = pool2_tmp(x * p2_stride_x, y * p2_stride_y, z, n);
    
    // schedule
    conv2.compute_root();
    tanh2.compute_root();
    pool2_tmp.compute_root();
    pool2.compute_root();
    
    // first fc
    // pool2: (4, 4, 50, 4) -> (800, 4)

    // Func flatten("flatten");
    // flatten(x, y) = pool2(x, y, z, n);

    Func fc1("fc1");
    RDom r_fc1(0, weight_fc1.dim(1).extent()); // (0, 800)
    fc1(x, n) += pool2(r_fc1.x, 0, 0, n) * weight_fc1(x, r_fc1.x);

    Func tanh3("tanh3");
    tanh3(x, y) = tanh(fc1(x, y));
    
    fc1.compute_root();
    tanh3.compute_root();

    // second fc
    Func fc2("fc2");
    RDom r_fc2(0, weight_fc2.dim(1).extent());
    fc2(x, n) += tanh3(r_fc2.x, n) * weight_fc2(x, r_fc2.x);

    fc2.compute_root();

    // softmax
    RDom r_softmax(0, 10);
    Func exp_vals;
    exp_vals(x, n) = exp(fc2(x, n));
    Func final("final");
    final(x, n) = exp_vals(x, n) / sum(exp_vals(r_softmax.x, n));

    final.compute_root();

    Buffer<uint16_t> output(10, 1000);


    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }    

    final.compile_to_lowered_stmt("lenet.stmt", {input, weight_conv1, weight_conv2, weight_fc1, weight_fc2}, Text);

    final.compile_to_heterocl("lenet.py", {input, weight_conv1, weight_conv2, weight_fc1, weight_fc2}, output_shape, "final"); // with some bugs

    return 0;
}





