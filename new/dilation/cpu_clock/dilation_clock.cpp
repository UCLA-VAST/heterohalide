// On linux, you can compile and run it like so:
// g++ blur.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/codegen/blur
#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    ImageParam input(type_of<uint8_t>(), 2);


    Var x("x"), y("y");
    Func max_local("max_local");
    RDom box(0, 3, 0, 3);


    max_local(x, y) = maximum(input(x + box.x, y + box.y)); // local maximum
    Func final("final");
    final(x, y) = max_local(x, y);

    // CPU schedule - no schedule
    // max_local.compute_at(final, y)
    //          .vectorize(y, 8);
    

    max_local.compile_to_static_library("dilation_lib", {input}, "dilation");



    return 0;
}