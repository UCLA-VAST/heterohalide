// On linux, you can compile and run it like so:
// g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/codegen/blur
#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    ImageParam input(type_of<uint8_t>(), 2);


    Var x("x"), y("y");
    Func min_local("min_local");
    RDom box(0, 3, 0, 3);


    min_local(x, y) = minimum(input(x + box.x, y + box.y)); // local minimum
    Func final("final");
    final(x, y) = min_local(x, y);

    // CPU schedule - no schedule
    // min_local.compute_at(final, y)
    //          .vectorize(y, 8);
    
    min_local.compile_to_static_library("erosion_lib", {input}, "erosion");

    return 0;
}