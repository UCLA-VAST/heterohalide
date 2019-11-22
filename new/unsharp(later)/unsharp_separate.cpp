#include "Halide.h"
#include "fstream"
#include "halide_image_io.h"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 16; // 2448
    const int height = 16; // 3264
    Buffer<uint8_t> input_0(width, height);
    Buffer<uint8_t> input_1(width, height);
    Buffer<uint8_t> input_2(width, height);
    std::ifstream input_file ("/curr/jiajieli/app_stencil/unsharp/input_halide.txt");
    if (input_file.is_open())
    {   
        for (int y = 0; y < input_0.height(); y++) { 
            for (int x = 0; x < input_0.width(); x++){
                input_file >> input_0(x, y);
            }  
        }
        for (int y = 0; y < input_1.height(); y++) { 
            for (int x = 0; x < input_1.width(); x++){
                input_file >> input_1(x, y);
            }  
        }
        for (int y = 0; y < input_2.height(); y++) { 
            for (int x = 0; x < input_2.width(); x++){
                input_file >> input_2(x, y);
            }  
        }        
    }

    Var x("x"), y("y"), z("z"), c("c");
    Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

    // Buffer<uint8_t> input = Tools::load_image(argv[1]);
    // std::cout << "input width: " << input.width() << ", input height: " << input.height() << ", input dim 2:" << input.dim(2).extent() << "\n";

    
    Func kernel_f("kernel_f"), kernel("kernel"), gray("gray"), blur_x("blur_x");
    Func sharpen("sharpen"), ratio("ratio");
    Func hw_output_0("hw_output_0"), hw_output_1("hw_output_1"), hw_output_2("hw_output_2");
    Func output_0("output_0"), output_1("output_1"), output_2("output_2");

    RDom win2(0, 9, 0, 9);
    Func sum_y, sum_x("sum_x");

    // Define a 9x9 Gaussian Blur with a repeat-edge boundary condition.
    float sigma = 1.5f;

    kernel_f(x) = exp(-x*x/(2*sigma*sigma)) / (sqrtf(2*M_PI)*sigma);
    // normalize and convert to 8bit fixed point
    kernel(x) = cast<uint8_t>(kernel_f(x) * 255 /
                                (kernel_f(0) + kernel_f(1)*2 + kernel_f(2)*2
                                + kernel_f(3)*2 + kernel_f(4)*2));


    gray(x, y) = cast<uint8_t>((77 * cast<uint16_t>(input_0(x, y))
                                + 150 * cast<uint16_t>(input_1(x, y))
                                + 29 * cast<uint16_t>(input_2(x, y))) >> 8);


    // 2D filter: direct map
    sum_x(x, y) += cast<uint32_t>(gray(x + win2.x, y + win2.y)) * kernel(win2.x - 4) * kernel(win2.y - 4);
    blur_x(x, y) = cast<uint8_t>(sum_x(x, y) >> 16);

    // sum_x.update(0).unroll(win2.x).unroll(win2.y);
    

    sharpen(x, y) = cast<uint8_t>(clamp(2 * cast<uint16_t>(gray(x, y)) - blur_x(x, y), 0, 255)); // seems only result in "min" in Halide IR, maybe becaude the data type is uint, so dont need the 0 as min constraint? 
    // Above: Actually have to change the cast type of inner loop to int16 instead of uint16, otherwise we will lose max syntax (clamp need to include max and min) is Halide IR
    ratio(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(sharpen(x, y)) * 32 / max(gray(x, y), 1), 0, 255));
    hw_output_0(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(ratio(x, y)) * input_0(x + 4, y + 4) / 32, 0, 255)); // x + 4, y + 4
    hw_output_1(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(ratio(x, y)) * input_1(x + 4, y + 4) / 32, 0, 255));
    hw_output_2(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(ratio(x, y)) * input_2(x + 4, y + 4) / 32, 0, 255));
    output_0(x, y) = hw_output_0(x, y);
    output_1(x, y) = hw_output_1(x, y);
    output_2(x, y) = hw_output_2(x, y);

    gray.compute_root();
    sum_x.compute_root();
    blur_x.compute_root();
    sharpen.compute_root();
    ratio.compute_root();
    hw_output_0.compute_root();
    hw_output_1.compute_root();
    hw_output_2.compute_root();
    output_0.compute_root();
    output_1.compute_root();
    output_2.compute_root();

    Buffer<uint8_t> out_0(input_0.width() - 8, input_0.height() - 8); // 2440, 3256

    // output.realize(out);
    // std::ofstream outfile ("/curr/jiajieli/app_stencil/unsharp/output_halide.txt");
    // if (outfile)
    // {
    //     for (int c = 0; c < out.dim(2).extent(); c++) {
    //         for (int y = 0; y < out.height(); y++) {
    //             for (int x = 0; x < out.width(); x++) {
    //                 outfile << out(x, y, c) << '\t';            
    //             }
    //         }
    //     }  
    // }

    Pipeline all_output({output_0, output_1, output_2});
    // all_output.compile_to_lowered_stmt("unsharp_separate_all.html", {input_0, input_1, input_2}, HTML);

    std::vector<int> output_0_shape;
    for (int i = 0; i < out_0.dimensions(); i++){
        output_0_shape.push_back(out_0.extent(i));
    }
    all_output.compile_to_heterocl("unsharp_separate_all.py", {input_0, input_1, input_2}, "output_0", output_0_shape);

    // output_0.compile_to_lowered_stmt("unsharp_separate_0.html", {input_0, input_1, input_2}, HTML);
    // output_1.compile_to_lowered_stmt("unsharp_separate_1.html", {input_0, input_1, input_2}, HTML);
    // output_2.compile_to_lowered_stmt("unsharp_separate_2.html", {input_0, input_1, input_2}, HTML);
    // std::cout << "HTML generated\n";
    // output_0.compile_to_heterocl("unsharp_separate_0_generate_test.py", {input_0, input_1, input_2}, output_0_shape, "output_0");
    // output_1.compile_to_heterocl("unsharp_separate_1_generate.py", {input_0, input_1, input_2}, output_0_shape, "output_1");
    // output_2.compile_to_heterocl("unsharp_separate_2_generate.py", {input_0, input_1, input_2}, output_0_shape, "output_2");
    // std::cout << "HeteroCL generated\n";

    return 0;
}