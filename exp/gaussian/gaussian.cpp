#include "Halide.h"
#include "halide_image_io.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 400; // ori: 12
    const int height = 600; // ori: 12
    Buffer<uint16_t> input(width, height);
    
    std::ifstream input_file ("/curr/jiajieli/new/gaussian/input_halide.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x,y); 
            }
        }
    }
    
    // for (int y = 0; y < input.height(); y++) {
    //     for (int x = 0; x < input.width(); x++) {
    //         input(x, y) = rand() % 256;
    //     }
    // }
    


    Var x("x"), y("y"), z("z"), c("c");
    Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

    Func kernel_f("kernel_f"), kernel("kernel"), blur_x("blur_x");
    Func final("final"), output_final("output_final");

    RDom win2(0, 9, 0, 9); // ori: -4, 9, -4, 9: -4~4, -4~4
    Func sum_x("sum_x");

    float sigma = 1.5f;

    kernel_f(x) = exp(-x*x/(2*sigma*sigma)) / (sqrtf(2*M_PI)*sigma);

    // Normalize and convert to 8bit fixed point.
    // Kernel values will inlined into  the blurring kernel as constant
    kernel(x) = cast<uint8_t>(kernel_f(x) * 255 /
                                (kernel_f(0) + kernel_f(1)*2 + kernel_f(2)*2
                                + kernel_f(3)*2 + kernel_f(4)*2));

    // 2D filter: direct map
    sum_x(x, y) += cast<uint32_t>(input(x + win2.x, y + win2.y)) * kernel(win2.x - 4) * kernel(win2.y - 4); // shape: -8, -8
    blur_x(x, y) = cast<uint16_t>(sum_x(x, y) >> 16); // divided by 65536(2**16)

    // sum_x.update(0).unroll(win2.x).unroll(win2.y);

    output_final(x, y) = blur_x(x, y);
    final(x, y) = output_final(x, y);

    blur_x.compute_root();
    output_final.compute_root();
    final.compute_root();

    // if (argc == 1) {
    //     // kernel_f.compute_root();
    //     // kernel.compute_root();
    //     // sum_x.compute_root();
    //     blur_x.compute_root();
    //     output_final.compute_root();
    //     final.compute_root();

    //     sum_x.unroll_hcl(x, 8);
    //     blur_x.unroll_hcl(x, 8);
    //     output_final.unroll_hcl(x, 8);
    //     final.unroll_hcl(x, 8);
    //     // sum_x.unroll(x, 8);
    // }

    // if (argc == 2) { // CPU scheduling
    //     final.tile(x, y, xo, yo, xi, yi, 256, 64)
    //     .vectorize(xi, 8)
    //     .fuse(xo, yo, xo).parallel(xo);
    // }

    Buffer<uint16_t> output(input.width() - 8, input.height() - 8);


    // const clock_t begin_time = clock();
    // final.realize(output);
    // float total_time = float( clock() - begin_time ) / CLOCKS_PER_SEC;
    // std::ofstream clockfile ("clock.txt", std::ios::app);
    // if (clockfile) {
    //     clockfile << total_time << "\t";
    // }

    // generate Halide output

    // final.realize(output);
    // std::ofstream outfile ("/curr/jiajieli/new/gaussian/output_halide.txt");
    // if (outfile)
    // {
    //     for (int y = 0; y < output.height(); y++) {
    //         for (int x = 0; x < output.width(); x++) {
    //             outfile << output(x, y) << '\t';            
    //         }
    //     }  
    // }

    // generate HeteroCL code

    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }
    // final.compile_to_lowered_stmt("gaussian.stmt", {input}, Text);
    // std::cout << "Text generated\n";
    final.compile_to_heterocl("gaussian_gen.py", {input}, output_shape, "final");

    return 0;
}