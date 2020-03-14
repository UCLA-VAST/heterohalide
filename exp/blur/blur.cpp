// On linux, you can compile and run it like so:
// g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/codegen/blur
#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 648; // 648
    const int height = 482; // 482
    Buffer<uint16_t> input(width, height);

    // input:rand_matrix generated by rand_generator.py
    std::ifstream input_file ("/curr/jiajieli/new/blur/input.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x, y); 
            }
        }
    }
    
    Var x("x"), y("y");
    
    Func blur_x("blur_x");
    blur_x(x, y) = (input(x, y) + input(x+1, y) + input(x+2, y)) / 3;

    Func blur_y("blur_y");
    blur_y(x, y) = (blur_x(x, y) + blur_x(x, y+1) + blur_x(x, y+2)) / 3; 

    Func final("final");
    final(x, y) = blur_y(x, y);
    
    blur_x.compute_root();
    blur_y.compute_root();
    final.compute_root();

    // reorder, unroll, parallel
    // blur_x.reorder(y, x);
    // blur_x.unroll_hcl(x, 4);
    // blur_y.unroll_hcl(x, 4); 
    // blur_x.parallel(y);
    // blur_y.parallel(y);
   
    // // fuse
    // Var fused_blur_x("fused_blur_x");
    // blur_x.fuse(x, y, fused_blur_x);
    // Var fused_blur_y("fused_blur_y");
    // blur_y.fuse(x, y, fused_blur_y);

    // // split
    // Var x_outer("x_outer"), x_inner("x_inner");
    // blur_x.split(x, x_outer, x_inner, 200);
    // Var y_outer("y_outer"), y_inner("y_inner");
    // blur_y.split(y, y_outer, y_inner, 100);

    // // tile
    // Var x_inner("x_inner"), y_inner("y_inner");
    // blur_x.tile(x, y, x_inner, y_inner, 4, 4);

    
    Buffer<uint16_t> output(input.width() - 8, input.height() - 2); // width - 8, height - 2
    
    // generate Halide output
    // final.realize(output);    
    // std::ofstream outfile ("/curr/jiajieli/new/blur/output_halide.txt");
    // if (!outfile)
    // {
    //     std::cout << "can't open" << std::endl;
    // }
    // else
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


    final.compile_to_heterocl("blur_gen.py", {input}, output_shape, "final"); // add a parameter to send the output buffer shape into the CodeGen_HeteroCL
    std::cout << "HeteroCL code Generated" << std::endl;


    return 0;
}