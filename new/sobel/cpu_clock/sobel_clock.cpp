// On linux, you can compile and run it like so:
// g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/codegen/blur
#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    ImageParam input(type_of<int16_t>(), 2);
    ImageParam filter_x(type_of<int16_t>(), 2);
    ImageParam filter_y(type_of<int16_t>(), 2);

    filter_x(0, 0) = filter_x(2, 0) = -1;
    filter_x(0, 2) = filter_x(2, 2) = 1;
    filter_x(1, 0) = -3; filter_x(1, 2) = 3;
    filter_x(0, 1) = filter_x(1, 1) = filter_x(2, 1) = 0;

    filter_y(0, 0) = filter_y(0, 2) = -1;
    filter_y(2, 0) = filter_y(2, 2) = 1;
    filter_y(0, 1) = -3; filter_y(2, 1) = 3;
    filter_y(1, 0) = filter_y(1, 1) = filter_y(1, 2) = 0;

    Var x("x"), y("y");
    Func gx("gx"), gy("gy");
    Func final("final");
    RDom r(0, 3, 0, 3);
    gx(x, y) += filter_x(r.x, r.y) * input(x + r.x, y + r.y);
    gy(x, y) += filter_y(r.x, r.y) * gx(x + r.x, y + r.y);
    final(x, y) = gy(x, y);

    // CPU schedule - no schedule
    // gx.compute_at(gy, y)
    //          .vectorize(y, 8);
    // gy.compute_at(final, y)
    //          .vectorize(y, 8);
    
    gy.compile_to_static_library("sobel_lib", {input, filter_x, filter_y}, "sobel");

    return 0;
}