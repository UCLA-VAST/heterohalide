#include "Halide.h"
#include "fstream"
using namespace Halide;
int main(int argc, char **argv) {
    const int width = 4;
    const int height = 4;
    // Buffer<int16_t> input(width, height);
    // std::ifstream input_file ("/curr/jiajieli/app_stencil/unsharp/input_test_halide.txt");
    // if (input_file.is_open())
    // {   
    //     for (int y = 0; y < input.height(); y++) { 
    //         for (int x = 0; x < input.width(); x++){
    //             input_file >> input(x, y);
    //             std::cout << input(x, y) << "\t";
    //         }  
    //     }     
    // }

    Buffer<uint16_t> input_1(width, height);
    std::ifstream input_file_1 ("/curr/jiajieli/app_stencil/unsharp/input_test_1_halide.txt");
    if (input_file_1.is_open())
    {   
        for (int y = 0; y < input_1.height(); y++) { 
            for (int x = 0; x < input_1.width(); x++){
                input_file_1 >> input_1(x, y);
                std::cout << input_1(x, y) << "\t";
            }  
        }     
    }    
    std::cout << "\n";

    Buffer<uint16_t> input_2(width, height);
    std::ifstream input_file_2 ("/curr/jiajieli/app_stencil/unsharp/input_test_2_halide.txt");
    if (input_file_2.is_open())
    {   
        for (int y = 0; y < input_2.height(); y++) { 
            for (int x = 0; x < input_2.width(); x++){
                input_file_2 >> input_2(x, y);
                std::cout << input_2(x, y) << "\t";
            }  
        }     
    }    
    std::cout << "\n";

    Var x("x"), y("y");
    Func output("output");
    // output(x, y) = cast<uint16_t>(input(x, y));
    output(x, y) = clamp(input_1(x, y) - input_2(x, y), 0, 255);

    Buffer<uint16_t> out(width, height);
    output.realize(out);

    std::ofstream outfile ("/curr/jiajieli/app_stencil/unsharp/output_test_0_halide.txt");
    if (outfile)
    {
        for (int y = 0; y < out.height(); y++) {
            for (int x = 0; x < out.width(); x++) {
                outfile << out(x, y) << '\t';       
                std::cout << out(x, y) << "\t";     
            }
        }
    }
    std::cout << "\n";

    
}