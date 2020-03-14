#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 642;
    const int height = 482;
    Buffer<uint16_t> input(width, height);
    Var x("x"), y("y");
    Func max_local("max_local");
    RDom box(0, 3, 0, 3);

    std::ifstream input_file ("/curr/jiajieli/new/dilation/input.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x, y); 
            }
        }
    }

    max_local(x, y) = maximum(input(x + box.x, y + box.y)); // local maximum
    
    Func final("final");
    final(x, y) = max_local(x, y);
    
    // // CPU schedule same or similar with conv_layer in Halide
    // max_local.compute_at(final, y)
    //          .vectorize(y, 8);

    max_local.compute_root();
    final.compute_root();

    Buffer<uint16_t> output(input.width() - 2, input.height() - 2);

    // generate Halide output

    // final.realize(output);    
    // std::ofstream outfile ("/curr/jiajieli/new/dilation/output_halide.txt");
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

    // final.compile_to_lowered_stmt("dilation.stmt", {input}, Text);
    // std::cout << "STMT Generated\n";
    final.compile_to_heterocl("dilation_gen.py", {input}, output_shape, "final"); // need some more modification to successfully run Code Generator, but it's doable
    
}