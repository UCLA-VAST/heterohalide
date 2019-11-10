#include "Halide.h"
#include "halide_image_io.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    // Buffer<uint8_t> in = Tools::load_image(argv[1]);

    const int width = 4000; // ori: 12
    const int height = 6000; // ori: 12
    Buffer<uint8_t> input(width, height);
    
    // std::ifstream input_file ("/curr/jiajieli/app_stencil/gaussian/input_halide.txt");
    // if (input_file.is_open())
    // {     
    //     for (int y = 0; y < input.height(); y++){
    //         for (int x = 0; x < input.width(); x++){
    //             input_file >> input(x,y); 
    //         }
    //     }
    // }
    
    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++) {
            input(x, y) = rand() % 256;
        }
    }
    


    Var x("x"), y("y"), z("z"), c("c");
    Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

    Func kernel_f("kernel_f"), kernel("kernel"), blur_x("blur_x");
    Func output("output"), hw_output("hw_output");

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
    blur_x(x, y) = cast<uint8_t>(sum_x(x, y) >> 16); // divided by 65536(2**16)

    // sum_x.update(0).unroll(win2.x).unroll(win2.y);

    hw_output(x, y) = blur_x(x, y);
    output(x, y) = hw_output(x, y);

    if (argc == 1) {
        // kernel_f.compute_root();
        // kernel.compute_root();
        // sum_x.compute_root();
        blur_x.compute_root();
        hw_output.compute_root();
        output.compute_root();

        sum_x.unroll_hcl(x, 8);
        blur_x.unroll_hcl(x, 8);
        hw_output.unroll_hcl(x, 8);
        output.unroll_hcl(x, 8);
        // sum_x.unroll(x, 8);
    }

    if (argc == 2) { // CPU scheduling
        output.tile(x, y, xo, yo, xi, yi, 256, 64)
        .vectorize(xi, 8)
        .fuse(xo, yo, xo).parallel(xo);
    }
    Buffer<uint8_t> out(input.width()-8, input.height()-8);


    const clock_t begin_time = clock();
    output.realize(out);
    float total_time = float( clock() - begin_time ) / CLOCKS_PER_SEC;
    std::ofstream clockfile ("clock.txt", std::ios::app);
    if (clockfile) {
        clockfile << total_time << "\t";
    }

    // std::ofstream outfile ("/curr/jiajieli/app_stencil/gaussian/output_halide.txt");
    // if (outfile)
    // {
    //     for (int y = 0; y < out.height(); y++) {
    //         for (int x = 0; x < out.width(); x++) {
    //             outfile << out(x, y) << '\t';            
    //         }
    //     }  
    // }



    // std::vector<int> output_shape;
    // for (int i = 0; i < out.dimensions(); i++){
    //     output_shape.push_back(out.extent(i));
    // }
    // output.compile_to_lowered_stmt("gaussian_unroll.html", {input}, HTML);
    // std::cout << "HTML generated\n";
    // output.compile_to_heterocl("gaussian_generate.py", {input}, output_shape, "output");

    return 0;
}