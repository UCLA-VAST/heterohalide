#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 642;
    const int height = 482;
    Buffer<uint16_t> input(width, height);
    Var x("x"), y("y");
    Func min_local("min_local");
    RDom box(0, 3, 0, 3);

    std::ifstream input_file ("/curr/jiajieli/new/erosion/input.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x, y); 
            }
        }
    }

    min_local(x, y) = minimum(input(x + box.x, y + box.y)); // local minimum
    Func final("final");
    final(x, y) = min_local(x, y);
    
    // // CPU schedule same or similar with conv_layer in Halide
    // min_local.compute_at(final, y)
    //          .vectorize(y, 8);

    min_local.compute_root();
    final.compute_root();
    final.unroll(x, 4);

    Buffer<uint16_t> output(input.width() - 2, input.height() - 2);
    
    // generate Halide output

    // final.realize(output);    
    // std::ofstream outfile ("/curr/jiajieli/new/erosion/output_halide.txt");
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

    final.compile_to_lowered_stmt("erosion_unroll.stmt", {input}, Text);
    std::cout << "HTML Generated\n";
    final.compile_to_heterocl("erosion_gen_unroll.py", {input}, output_shape, "final"); // need some more modification to succefully run Code Generator, but it's doable
    
}