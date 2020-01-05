 // g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o conv_layer
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/conv/conv_layer

#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    Buffer<float> input(67, 67, 32, 4); // width, height, channels, dim(3).extent (batch)
    Buffer<float> filter(3, 3, 32, 32);
    Buffer<float> bias(32);

    std::ifstream input_file ("/curr/jiajieli/new/conv/input.txt");
    for (int c = 0; c < input.dim(3).extent(); c++) {
        for (int z = 0; z < input.channels(); z++) {
            for (int y = 0; y < input.height(); y++) {
                for (int x = 0; x < input.width(); x++) {
                    input_file >> input(x, y, z, c);
                }
            }
        }
    }

    std::ifstream filter_file ("/curr/jiajieli/new/conv/filter.txt");
    for (int c = 0; c < filter.dim(3).extent(); c++) {
        for (int z = 0; z < filter.channels(); z++) {
            for (int y = 0; y < filter.height(); y++) {
                for (int x = 0; x < filter.width(); x++) {
                    filter_file >> filter(x, y, z, c);
                }
            }
        }
    }

    std::ifstream bias_file ("/curr/jiajieli/new/conv/bias.txt");
    for (int x = 0; x < bias.width(); x++) {
        bias_file >> bias(x);
    }
  
    Var x("x"), y("y"), z("z"), n("n");
    RDom r(filter.dim(0).min(), filter.dim(0).extent(),
            filter.dim(1).min(), filter.dim(1).extent(),
            filter.dim(2).min(), filter.dim(2).extent());

    Func f_conv("f_conv");
    f_conv(x, y, z, n) = bias(z);
    f_conv(x, y, z, n) += filter(r.x, r.y, r.z, z) * input(x + r.x, y + r.y, r.z, n);

    Func f_ReLU("f_relu");
    f_ReLU(x, y, z, n) = max(0, f_conv(x, y, z, n));

    Func final("final");
    final(x, y, z, n) = f_ReLU(x, y, z, n);

    // Func f_conv("f_conv");
    // f_conv(x, y, z, n) += filter(r.x, r.y, r.z, z) * input(x + r.x, y + r.y, r.z, n);
    
    // Func f_bias("f_bias");
    // f_bias(x, y, z, n) = f_conv(x, y, z, n) + bias(z);

    // Func f_ReLU("f_relu");
    // f_ReLU(x, y, z, n) = max(0, f_bias(x, y, z, n));

    // Func final("final");
    // final(x, y, z, n) = f_ReLU(x, y, z, n);

    // Var xi{"xi"}, yi{"yi"}, zo{"zo"}, zi{"zi"};
    // f_conv.tile(x, y, xi, yi, 4, 4);
    // f_conv.compute_root();
    // f_ReLU.compute_root();
    // final.compute_root();


    // test on CPU schedule
    Var z_t("z_t"), y_t("y_t"), par("par");
    int vec_len = 8;
    int o_block_size = 32;
    int y_block = 32;
    f_conv.compute_root();
    f_conv.fuse(z, n, par).parallel(par);
    f_conv.update()
        .reorder(x, y, r.z)
        .split(y, y, y_t, y_block)
        .split(z, z, z_t, o_block_size)
        .reorder(y_t, z_t, y, r.z, z)
        .unroll(r.x, 3)
        .unroll(r.y, 3)
        .fuse(z, n, par)
        .parallel(par);



    Buffer<float> output(64, 64, 32, 4);

    // output.realize(out);
    // std::ofstream output_file ("/curr/jiajieli/new/conv/output_halide.txt");
    // for (int c = 0; c < out.dim(3).extent(); c++) {
    //     for (int z = 0; z < out.channels(); z++) {
    //         for (int y = 0; y < out.height(); y++) {
    //             for (int x = 0; x < out.width(); x++) {
    //                 output_file << out(x, y, z, c) << "\t";
    //             }
    //         }
    //     }
    // }    




    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }

    final.compile_to_lowered_stmt("conv_tile_test.stmt", {input, filter, bias}, Text);
    final.compile_to_heterocl("conv_gen_tile_test.py", {input, filter, bias}, output_shape, "final");


    std::cout << "HeteroCL code generated" << std::endl;



    return 0;
}