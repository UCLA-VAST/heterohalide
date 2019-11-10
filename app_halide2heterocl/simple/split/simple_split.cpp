// On linux, you can compile and run it like so:
// g++ simple.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o simple
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/simple/simple

#include "Halide.h"
#include <stdio.h>
#include "fstream"

// This time we'll just import the entire Halide namespace
using namespace Halide;

int main(int argc, char **argv) {
    Buffer<int> input(10, 8);

    std::ifstream input_file ("/curr/jiajieli/app_halide2heterocl/simple/input.txt");
    if (input_file.is_open())
    {     
        for (int y = 0; y < input.height(); y++){
            for (int x = 0; x < input.width(); x++){
                input_file >> input(x,y); 
            }
        }
    }
    
    Func gradient("output");
    Var x("x"), y("y");
    gradient(x, y) = input(x, y) + input(x, y);

    Buffer<int> output(input.width(), input.height());
    // gradient.realize(output);

    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }

    // Internal::CodeGen_HeteroCL cg = gradient.compile_to_heterocl("simple_split_generate.py", {input}, output_shape, "simple_split");
    // gradient.compile_to_heterocl("simple_split_generate.py", {input}, output_shape, "simple_split");

    // Func B("next");
    // B(x, y) = gradient(x, y) + 1;

    // // compute_at: done before lower
    // gradient.compute_at(B, y);

    // gradient.trace_stores();

    // // reorder: done before lower. Default: inner x, outer y
    // gradient.reorder(y, x); // inner y, outer x

    // split: done before lower
    Var x_outer, x_inner;
    gradient.split(x, x_outer, x_inner, 2);

    // // unroll: done in lower
    // gradient.unroll(x, 2);

    // // vectorize: done in lower
    // gradient.vectorize(x, 2);

    // // fuse: done before lower
    // Var fused;
    // gradient.fuse(x, y, fused);

    // // tile: done before lower
    // Var x_outer, y_outer, x_inner, y_inner;
    // gradient.tile(x, y, x_outer, y_outer, x_inner, y_inner, 4, 4);

    // // fuse and parallel
    // Var fused;
    // gradient.fuse(x, y, fused);
    // gradient.parallel(fused);


    gradient.realize(output);
    std::ofstream output_file ("/curr/jiajieli/app_halide2heterocl/simple/split/output_halide.txt");
    if (output_file.is_open())
    {     
        for (int y = 0; y < output.height(); y++){
            for (int x = 0; x < output.width(); x++){
                output_file << output(x,y); 
            }
        }
    }
    // B.compile_to_lowered_stmt("two_func.stmt", {input}, Text); 
    // B.compile_to_heterocl("two_func.py", {input}, output_shape, "two_func");


    gradient.compile_to_lowered_stmt("simple_split.stmt", {input}, Text); 
    // gradient.compile_to_heterocl("simple_split_generate.py", {input}, output_shape, "simple_split"); // append write to the same file

    // gradient.compile_schedule_to_heterocl("simple_split_schedule.py", {})

    // gradient.compile_to_lowered_stmt("simple.html", {input}, HTML);
    // gradient.compile_to_c("simple.c", {input}, "simple");

    printf("Success!\n");
    return 0;
}
