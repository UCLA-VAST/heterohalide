#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 768; // 768
    const int height = 1280; // 1280
    const int channel = 3;
    Buffer<float> input(width, height, channel); // (w, h, c)

    std::ifstream input_file ("/curr/jiajieli/new/linear_blur/input.txt");

    for (int z = 0; z < input.channels(); z++) {
        for (int y = 0; y < input.height(); y++) {
            for (int x = 0; x < input.width(); x++) {
                input_file >> input(x, y, z);
            }
        }
    }

    // for (int z = 0; z < input.channels(); z++) {
    //     for (int y = 0; y < input.height(); y++) {
    //         for (int x = 0; x < input.width(); x++) {
    //             input(x, y, z) = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/256));;
    //         }
    //     }
    // }
    

    Var x{"x"}, y{"y"}, c{"c"};
    Func linear("linear");
    linear(x, y, c) = select(input(x, y, c) <= 0.04045f,
                                 input(x, y, c) / 12.92f,
                                 pow(((input(x, y, c) + .055f) / (1.0f + .055f)), 2.4f));

    Func blur_x{"blur_x"};
    blur_x(x, y, c) = (linear(x, y, c) + linear(x+1, y, c) + linear(x+2, y, c))/3;
    Func blur_y{"blur_y"};
    blur_y(x, y, c) = (blur_x(x, y, c) + blur_x(x, y+1, c) + blur_x(x, y+2, c))/3;
    
    Func srgb{"srgb"};
    srgb(x, y, c) = select(blur_y(x, y, c) <= .0031308f,
                    blur_y(x, y, c) * 12.92f,
                    (1 + .055f) * pow(blur_y(x, y, c), 1.0f / 2.4f) - .055f);
    Func final{"final"};
    final(x, y, c) = srgb(x, y, c);

    linear.unroll_hcl(y, 16);
    blur_x.reorder(y, x);
    blur_x.unroll_hcl(x, 4);
    blur_y.unroll_hcl(x, 4); 
    blur_x.parallel(y);
    blur_y.parallel(y);


    //fuse
    // Var fused("fused");
    // linear.fuse(x, y, fused);
    // blur_x.fuse(x, y, fused);
    // blur_y.fuse(x, y, fused);
    // srgb.fuse(x, y, fused);
    // output.fuse(x, y, fused);

    // // reorder
    // blur_y.reorder(c, y, x);
    // srgb.reorder(y, x, c);

    // // split
    // Var x_outer("x_outer"), x_inner("x_inner");
    // // srgb.split(x, x_outer, x_inner, 383);
    // blur_y.split(x, x_outer, x_inner, 383);

    // Var y_outer("y_outer"), y_inner("y_inner");
    // blur_x.split(y, y_outer, y_inner, 640);
    // unroll & reorder

    
    // compute_root: let each stage separated
    linear.compute_root();
    blur_x.compute_root();
    blur_y.compute_root();
    srgb.compute_root();
    final.compute_root();



    Buffer<float> output(width - 2, height - 2, channel);

    // output.realize(out);
    // std::ofstream output_file ("/curr/jiajieli/new/linear_blur/output_halide.txt");
    // for (int z = 0; z < out.channels(); z++) {
    //     for (int y = 0; y < out.height(); y++) {
    //         for (int x = 0; x < out.width(); x++) {
    //             output_file << out(x, y, z) << '\t';
    //         }
    //     }
    // }
    

    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }

    final.compile_to_lowered_stmt("linear_blur_schedule.stmt", {input}, Text);   
    final.compile_to_heterocl("linear_blur_schedule_gen.py", {input, }, output_shape, "final");
    std::cout << "HeteroCL code generated" << std::endl;


    return 0;
}