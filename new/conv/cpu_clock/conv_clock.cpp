// On linux, you can compile and run it like so:
// g++ conv.cpp -g -std=c++11 -I /curr/jiajieli/Halide/include -I /curr/jiajieli/Halide/tools -L /curr/jiajieli/Halide/bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o blur
// LD_LIBRARY_PATH=/curr/jiajieli/Halide/bin /curr/jiajieli/app_halide2heterocl/blur/codegen/blur
#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    ImageParam input(type_of<float>(), 4);
    ImageParam filter(type_of<float>(), 4);
    ImageParam bias(type_of<float>(), 1);
    
    Var x("x"), y("y"), z("z"), n("n");
    RDom r(filter.dim(0).min(), filter.dim(0).extent(),
            filter.dim(1).min(), filter.dim(1).extent(),
            filter.dim(2).min(), filter.dim(2).extent());

    Func f_conv("f_conv");
    f_conv(x, y, z, n) = bias(z);
    f_conv(x, y, z, n) += filter(r.x, r.y, r.z, z) * input(x + r.x, y + r.y, r.z, n);

    Func f_ReLU("f_relu");
    f_ReLU(x, y, z, n) = max(0, f_conv(x, y, z, n));

    Func final("final");
    final(x, y, z, n) = f_ReLU(x, y, z, n);

    final.compile_to_static_library("conv_lib", {input, filter, bias}, "conv");

    // float total_time = float( clock() - begin_time ) / CLOCKS_PER_SEC;
    // std::ofstream clockfile ("clock.txt", std::ios::app);
    // if (clockfile) {
    //     clockfile << total_time << "\t";
    // }
    

    return 0;
}