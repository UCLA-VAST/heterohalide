#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {

    const int width = 200;
    Buffer<uint16_t> input(width);
    
    std::ifstream input_file ("/curr/jiajieli/app_stencil/fanout/input_halide.txt");
    if (input_file.is_open())
    {     
        for (int x = 0; x < input.width(); x++){
            input_file >> input(x);
        }  
    }

    Var x("x");

    Func A("A");
    Func B("B");
    Func C("C");
    Func hw_output("hw_output");
    Func output("output");

    A(x) = input(x);
    B(x) = A(x) + A(x+1);
    C(x) = A(x+2);
    hw_output(x) = A(x) + A(x+1) + B(x) + B(x+1) + C(x);
    output(x) = hw_output(x);

    Buffer<uint16_t> out(input.width() - 2);

    output.realize(out);
    std::ofstream outfile ("/curr/jiajieli/app_stencil/fanout/output_halide.txt");
    if (outfile)
    {
        for (int x = 0; x < out.width(); x++) {
            outfile << out(x) << '\t';            
        }
    }

    // std::vector<int> output_shape;
    // for (int i = 0; i < out.dimensions(); i++){
    //     output_shape.push_back(out.extent(i));
    // }

    // output.compile_to_lowered_stmt("fanout.html", {input}, HTML);
    // std::cout << "HTML generated\n";
    // output.compile_to_heterocl("fanout_generate.py", {input}, output_shape, "output");

    return 0;
}