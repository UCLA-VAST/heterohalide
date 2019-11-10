#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    // const int width = 2000; // 768
    // const int height = 3000; // 1280
    // const int channel = 3;
    // Buffer<float> input(width, height, channel); // (w, h, c)
    ImageParam input(type_of<float>(), 3);

    Var x{"x"}, y{"y"}, c{"c"};
    Func linear("linear");
    linear(x, y, c) = select(input(x, y, c) <= 0.04045f,
                                 input(x, y, c) / 12.92f,
                                 pow(((input(x, y, c) + .055f) / (1.0f + .055f)), 2.4f));

    Func blur_x{"blur_x"};
    blur_x(x, y, c) = (linear(x, y, c) + linear(x+1, y, c) + linear(x+2, y, c))/3;
    Func blur_y{"blur_y"};
    blur_y(x, y, c) = (blur_x(x, y, c) + blur_x(x, y+1, c) + blur_x(x, y+2, c))/3;
    
    Func srgb{"srgb"};
    srgb(x, y, c) = select(blur_y(x, y, c) <= .0031308f,
                    blur_y(x, y, c) * 12.92f,
                    (1 + .055f) * pow(blur_y(x, y, c), 1.0f / 2.4f) - .055f);
    // Func output{"output"};
    // output(x, y, c) = srgb(x, y, c);

    // Func output("output");
    // output(x, y, c) = linear(x, y, c);

    // Var xi("xi"), yi1("yi1"), y1("y1");
    // linear.split(y, y, yi1, 8).parallel(y).vectorize(x, 8); // stage_1

    // Var yi2("yi2"), y2("y2");
    // blur_y.split(y, y2, yi2, 8).parallel(y2).vectorize(x, 8);
    // blur_x.store_at(blur_y, y2).compute_at(blur_y, yi2).vectorize(x, 8); // stage_2

    Var yi3("yi3"), y3("y3");
    srgb.split(y, y3, yi3, 8).parallel(y3).vectorize(x, 8); // stage_3


    srgb.compile_to_static_library("linear_blur_lib", {input}, "linear_blur");
    
    return 0;
}