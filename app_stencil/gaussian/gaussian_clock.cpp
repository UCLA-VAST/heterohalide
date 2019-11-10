// very weird...: if Buffer type is uint8_t instead of uin16_t, it can't be read/write using fstream correctly. But uint16_t is fine......
#include "Halide.h"
#include "halide_image_io.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {

    ImageParam input(type_of<uint8_t>(), 2);

    Var x("x"), y("y"), z("z"), c("c");
    Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

    Func kernel_f("kernel_f"), kernel("kernel"), blur_x("blur_x");
    Func output("output"), hw_output("hw_output");

    RDom win2(0, 9, 0, 9); // ori: -4, 9, -4, 9: -4~4, -4~4
    Func sum_x("sum_x");

    float sigma = 1.5f;

    kernel_f(x) = exp(-x*x/(2*sigma*sigma)) / (sqrtf(2*M_PI)*sigma);

    // Normalize and convert to 8bit fixed point.
    // Kernel values will inlined into  the blurring kernel as constant
    kernel(x) = cast<uint8_t>(kernel_f(x) * 255 /
                                (kernel_f(0) + kernel_f(1)*2 + kernel_f(2)*2
                                + kernel_f(3)*2 + kernel_f(4)*2));

    // 2D filter: direct map
    sum_x(x, y) += cast<uint32_t>(input(x + win2.x, y + win2.y)) * kernel(win2.x - 4) * kernel(win2.y - 4); // shape: -8, -8
    blur_x(x, y) = cast<uint8_t>(sum_x(x, y) >> 16); // divided by 65536(2**16)

    hw_output(x, y) = blur_x(x, y);
    output(x, y) = hw_output(x, y);

    output.tile(x, y, xo, yo, xi, yi, 256, 64)
            .vectorize(xi, 8)
            .fuse(xo, yo, xo).parallel(xo);
    
    output.compile_to_static_library("gaussian_lib", {input}, "gaussian");
}