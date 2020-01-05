// On linux, you can compile and run it like so:
// g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/codegen/blur
#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    ImageParam input(type_of<int16_t>(), 4); 
    ImageParam weight_conv1(type_of<int16_t>(), 4); 
    ImageParam weight_conv2(type_of<int16_t>(), 4); 
    ImageParam weight_fc1(type_of<int16_t>(), 2);
    ImageParam weight_fc2(type_of<int16_t>(), 2);

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


    Var z_t("z_t"), y_t("y_t"), par("par");
    int o_block_size = 4;
    int y_block = 4;
    conv1.compute_root();
    conv1.fuse(z, n, par).parallel(par);
    conv1.update()
        .reorder(x, y, r_conv1.z)
        .split(y, y, y_t, y_block)
        .split(z, z, z_t, o_block_size)
        .reorder(y_t, z_t, y, r_conv1.z, z)
        .unroll(r_conv1.x, 5)
        .unroll(r_conv1.y, 5)
        .fuse(z, n, par)
        .parallel(par);




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
    // pool2: (4, 4, 50, 1000) -> (800, 1000)

    // Func flatten("flatten");
    // flatten(x, y) = pool2(x, y, z, n);

    Func flat("flat");
    flat(x, n) = pool2(x % 4, x / 4 % 4, x / 4 / 4 % 50, n);

    Func fc1("fc1");
    RDom r_fc1(0, weight_fc1.dim(1).extent()); // (0, 800)
    fc1(x, n) += flat(r_fc1.x, n) * weight_fc1(x, r_fc1.x);

    Func tanh3("tanh3");
    tanh3(x, y) = tanh(fc1(x, y));
    
    // schedule
    // flat.compute_root();
    // fc1.compute_root();
    // tanh3.compute_root();

    flat.compute_root();
    fc1.compute_root();
    tanh3.compute_root();

    
    int block_size1 = 4;
    Var xi, xii, ni, nii;
    RVar ri_fc1;
    // fc1.vectorize(x, 4);
    // fc1.update() 
    //     .split(x, x, xi, block_size1).split(xi, xi, xii, 4) 
    //     .split(n, n, ni, block_size1).split(ni, ni, nii, 4) 
    //     .split(r_fc1, r_fc1, ri_fc1, block_size1) 
    //     .reorder(xii, nii, xi, ri_fc1, ni, r_fc1, x, n) 
    //     .parallel(n).vectorize(xii).unroll(xi).unroll(nii);
    
    // fc1.compute_root();
    // fc1.vectorize(x, 8);
    // fc1.parallel(n);
    // fc1.compute_root();



    // second fc
    Func fc2("fc2");
    RDom r_fc2(0, weight_fc2.dim(1).extent());
    fc2(x, n) += tanh3(r_fc2.x, n) * weight_fc2(x, r_fc2.x);

    int block_size2 = 4;
    RVar ri_fc2;
    // fc2.vectorize(x, 4);
    // fc2.update() 
    //     // .split(x, x, xi, block_size2).split(xi, xi, xii, 4) 
    //     .split(n, n, ni, block_size2).split(ni, ni, nii, 4) 
    //     .split(r_fc2, r_fc2, ri_fc2, block_size2) 
    //     .reorder(xii, nii, xi, ri_fc2, ni, r_fc2, x, n) 
    //     .parallel(n).vectorize(xii).unroll(xi).unroll(nii);
    // fc2.parallel(n);
    // fc2.vectorize(x, 8);
    // fc2.compute_root();


    // schedule
    fc2.compute_root();
    fc2.vectorize(n, 28);

    // softmax
    RDom r_softmax(0, 10);
    Func exp_vals;
    exp_vals(x, n) = exp(fc2(x, n));
    Func final("final");
    final(x, n) = exp_vals(x, n) / sum(exp_vals(r_softmax.x, n));
    
    final.compute_root();
    final.vectorize(n, 28);

    final.compile_to_static_library("lenet_lib", {input, weight_conv1, weight_conv2, weight_fc1, weight_fc2}, "lenet");



    return 0;
}