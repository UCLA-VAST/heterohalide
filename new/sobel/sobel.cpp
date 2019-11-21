#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 642;
    const int height = 482;

    Buffer<int16_t> input(width, height);
    std::ifstream input_file ("/curr/jiajieli/new/sobel/input.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x, y); 
            }
        }
    }

    Buffer<int16_t> filter_x(3, 3);
    std::ifstream filter_x_file ("/curr/jiajieli/new/sobel/filter_x.txt");
    if (filter_x_file.is_open())
    {     
        for (int y = 0; y < filter_x.height(); y++){
            for (int x = 0; x < filter_x.width(); x++){
                filter_x_file >> filter_x(x, y); 
            }
        }
    }

    Buffer<int16_t> filter_y(3, 3);
    std::ifstream filter_y_file ("/curr/jiajieli/new/sobel/filter_y.txt");
    if (filter_y_file.is_open())
    {     
        for (int y = 0; y < filter_y.height(); y++){
            for (int x = 0; x < filter_y.width(); x++){
                filter_y_file >> filter_y(x, y); 
            }
        }
    }

    // Assignment Manually

    // filter_x(0, 0) = filter_x(2, 0) = -1;
    // filter_x(0, 2) = filter_x(2, 2) = 1;
    // filter_x(1, 0) = -3; filter_x(1, 2) = 3;
    // filter_x(0, 1) = filter_x(1, 1) = filter_x(2, 1) = 0;

    // filter_y(0, 0) = filter_y(0, 2) = -1;
    // filter_y(2, 0) = filter_y(2, 2) = 1;
    // filter_y(0, 1) = -3; filter_y(2, 1) = 3;
    // filter_y(1, 0) = filter_y(1, 1) = filter_y(1, 2) = 0;

    Var x("x"), y("y");
    Func gx("gx"), gy("gy");
    Func final("final");
    RDom r(0, 3, 0, 3);
    gx(x, y) += filter_x(r.x, r.y) * input(x + r.x, y + r.y);
    gy(x, y) += filter_y(r.x, r.y) * gx(x + r.x, y + r.y);
    final(x, y) = gy(x, y);

    
    // CPU schedule
    // gx.compute_at(gy, y)
    //             .vectorize(y, 8);
    // gy.compute_at(final, y)
    //          .vectorize(y, 8);

    // For HeteroCL schedule
    gx.compute_root();
    gy.compute_root();
    final.compute_root();

    Buffer<int16_t> output(input.width() - 4, input.height() - 4);

    // generate Halide output

    // final.realize(output);    
    // std::ofstream outfile ("/curr/jiajieli/new/sobel/output_halide.txt");
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

    final.compile_to_lowered_stmt("sobel.stmt", {input, filter_x, filter_y}, Text);
    std::cout << "STMT Generated\n";
    final.compile_to_heterocl("sobel_generate.py", {input, filter_x, filter_y}, output_shape, "final"); // need some more modification to succefully run Code Generator, but it's doable
    
}