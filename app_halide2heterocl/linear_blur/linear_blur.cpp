#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 2000; // 768
    const int height = 3000; // 1280
    const int channel = 3;
    Buffer<float> input(width, height, channel); // (w, h, c)

    // std::ifstream input_file ("/curr/jiajieli/app_halide2heterocl/linear_blur/input.txt");

    // for (int z = 0; z < input.channels(); z++) {
    //     for (int y = 0; y < input.height(); y++) {
    //         for (int x = 0; x < input.width(); x++) {
    //             input_file >> input(x, y, z);
    //         }
    //     }
    // }
    for (int z = 0; z < input.channels(); z++) {
        for (int y = 0; y < input.height(); y++) {
            for (int x = 0; x < input.width(); x++) {
                input(x, y, z) = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/256));;
            }
        }
    }
    

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
    Func output{"output"};
    output(x, y, c) = srgb(x, y, c);

    //fuse
    Var fused("fused");
    linear.fuse(x, y, fused);
    blur_x.fuse(x, y, fused);
    blur_y.fuse(x, y, fused);
    srgb.fuse(x, y, fused);
    output.fuse(x, y, fused);

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
    output.compute_root();

    if (argc == 2) { // CPU schedule
        std::cout << "CPU scheduling \n";
        linear.unroll(x, 8);
        // Var xi("xi"), yi1("yi1"), y1("y1");
        // linear.split(y, y, yi1, 8).parallel(y).vectorize(x, 8); // stage_1

        // Var yi2("yi2"), y2("y2");
        // blur_y.split(y, y2, yi2, 8).parallel(y2).vectorize(x, 8);
        // blur_x.store_at(blur_y, y2).compute_at(blur_y, yi2).vectorize(x, 8); // stage_2

        // Var yi3("yi3"), y3("y3");
        // srgb.split(y, y3, yi3, 8).parallel(y3).vectorize(x, 8); // stage_3
    }

    Buffer<float> out(width - 2, height - 2, channel);

    if (argc == 2) {
        const clock_t begin_time = clock();
        output.realize(out);
        float total_time = float( clock() - begin_time ) / CLOCKS_PER_SEC;
        std::ofstream clockfile ("clock.txt", std::ios::app);
        if (clockfile) {
            clockfile << total_time << "\t";
        }
    }



    // std::ofstream output_file ("/curr/jiajieli/app_halide2heterocl/linear_blur/output_halide_split.txt");
    // for (int z = 0; z < output.channels(); z++) {
    //     for (int y = 0; y < output.height(); y++) {
    //         for (int x = 0; x < output.width(); x++) {
    //             output_file << output(x, y, z) << '\t';
    //         }
    //     }
    // }
    
    if (argc == 1) {
        std::vector<int> output_shape;
        for (int i = 0; i < out.dimensions(); i++){
            output_shape.push_back(out.extent(i));
        }

        output.compile_to_lowered_stmt("linear_blur_fused_storaget_flatten.stmt", {input}, Text);   
        // output.compile_to_heterocl("linear_blur_generate.py", {input, }, output_shape, "output");
        // std::cout << "HeteroCL code generated" << std::endl;
    }

    return 0;
}