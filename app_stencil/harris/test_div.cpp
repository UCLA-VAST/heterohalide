#include "Halide.h"
#include "fstream"
using namespace Halide;
int main(int argc, char **argv) {
    Buffer<int16_t> input(4, 4);

    std::ifstream input_file ("/curr/jiajieli/app_stencil/harris/input_test.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x,y); 
            }
        }
    } 
    Var x("x"), y("y");
    Func output("output");
    int scale = 144;
    output(x, y) = cast<float>(input(x, y) / scale);
    // output(x, y) = cast<float>(input(x, y) / scale);
    output.compute_root();

    Buffer<float> out(input.width(), input.height());


    output.realize(out);
    std::ofstream outfile ("/curr/jiajieli/app_stencil/harris/output_test.txt");
    outfile.setf(std::ios::fixed, std::ios::floatfield);
    outfile.precision(5);
    if (outfile)
    {
        for (int y = 0; y < out.height(); y++) {
            for (int x = 0; x < out.width(); x++) {
                outfile << out(x, y) << '\t';            
            }
        }
    }

    // std::vector<int> output_shape;
    // for (int i = 0; i < out.dimensions(); i++){
    //     output_shape.push_back(out.extent(i));
    // }

    // output.compile_to_lowered_stmt("test.stmt", {input}, Text);
    // std::cout << "HTML Generated\n";
    // output.compile_to_heterocl("test_generate.py", {input}, output_shape, "output");

}