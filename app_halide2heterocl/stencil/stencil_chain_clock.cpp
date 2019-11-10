#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {

    int stencils = 3; //value, min, max. here iterations = 5
    ImageParam input(type_of<uint16_t>(), 2);


    std::vector<Func> stages;

    Var x("x"), y("y");

    Func f("initial");
    f(x, y) = input(x, y);

    stages.push_back(f);
    for (int s = 0; s < (int)stencils; s++) {
        Func f("stage_" + std::to_string(s));
        Expr e = cast<uint16_t>(0);
        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                e += ((i+3)*(j+3))*stages.back()(x+i+2, y+j+2);
            }
        }
        f(x, y) = e;
        stages.push_back(f);
    }

    Func output("output");
    output(x, y) = stages.back()(x, y);



    Var yi, yo, xo, xi, t;
    for (size_t i = 1; i < stages.size() - 1; i++) {
        Func s = stages[i];
        s.store_at(output, t).compute_at(output, yi).vectorize(s.args()[0], 16);
    }
    output.compute_root()
        .tile(x, y, xo, yo, xi, yi, 512, 512)
        .fuse(xo, yo, t)
        .parallel(t)
        .vectorize(xi, 16);
    

    output.compile_to_static_library("stencil_chain_lib", {input}, "stencil_chain");



    return 0;
}

