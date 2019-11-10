#include "Halide.h"
#include "fstream"
#include "halide_image_io.h"
using namespace Halide;

int main(int argc, char **argv) {


    ImageParam input(type_of<uint8_t>(), 3);

    Var x("x"), y("y"), z("z"), c("c");
    Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

    Func kernel_f("kernel_f"), kernel("kernel"), gray("gray"), blur_x("blur_x");
    Func sharpen("sharpen"), ratio("ratio");
    Func output("output"), hw_output("hw_output");

    RDom win2(0, 9, 0, 9);
    Func sum_y, sum_x("sum_x");

    // Define a 9x9 Gaussian Blur with a repeat-edge boundary condition.
    float sigma = 1.5f;

    kernel_f(x) = exp(-x*x/(2*sigma*sigma)) / (sqrtf(2*M_PI)*sigma);
    // normalize and convert to 8bit fixed point
    kernel(x) = cast<uint8_t>(kernel_f(x) * 255 /
                                (kernel_f(0) + kernel_f(1)*2 + kernel_f(2)*2
                                + kernel_f(3)*2 + kernel_f(4)*2));


    gray(x, y) = cast<uint8_t>((77 * cast<uint16_t>(input(x, y, 0))
                                + 150 * cast<uint16_t>(input(x, y, 1))
                                + 29 * cast<uint16_t>(input(x, y, 2))) >> 8);


    // 2D filter: direct map
    sum_x(x, y) += cast<uint32_t>(gray(x + win2.x, y + win2.y)) * kernel(win2.x - 4) * kernel(win2.y - 4);
    blur_x(x, y) = cast<uint8_t>(sum_x(x, y) >> 16);

    // sum_x.update(0).unroll(win2.x).unroll(win2.y);
    

    sharpen(x, y) = cast<uint8_t>(clamp(2 * cast<uint16_t>(gray(x, y)) - blur_x(x, y), 0, 255)); // seems only result in "min" in Halide IR, maybe becaude the data type is uint, so dont need the 0 as min constraint? 
    // Above: Actually have to change the cast type of inner loop to int16 instead of uint16, otherwise we will lose max syntax (clamp need to include max and min) is Halide IR
    ratio(x, y) = cast<uint8_t>(clamp(cast<uint16_t>(sharpen(x, y)) * 32 / max(gray(x, y), 1), 0, 255));
    hw_output(x, y, c) = cast<uint8_t>(clamp(cast<uint16_t>(ratio(x, y)) * input(x + 4, y + 4, c) / 32, 0, 255)); // x + 4, y + 4
    output(x, y, c) = hw_output(x, y, c);

    output.tile(x, y, xo, yo, xi, yi, 240, 360);
    gray.compute_at(output, xo).vectorize(x, 8);
    //blur_y.compute_at(output, xo).vectorize(x, 8);
    ratio.compute_at(output, xo).vectorize(x, 8);
    output.vectorize(xi, 8).reorder(xi, yi, c, xo, yo);

    output.fuse(xo, yo, xo).parallel(xo);

    output.compile_to_static_library("unsharp_lib", {input}, "unsharp");



    return 0;
}