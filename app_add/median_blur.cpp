#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 6480;
    const int height = 4820;
    Buffer<uint8_t> input(width, height);
    Var x("x"), y("y");
    Func hw_output("hw_output");
    RDom box(0, 3, 0, 3);
    hw_output(x, y) += input(x + box.x, y + box.y) / (3 * 3); // median_blur.
    Func output("output");
    output(x, y) = hw_output(x, y);
    
    // CPU schedule
    // hw_output.compute_at(output, y)
    //          .vectorize(y, 8);

    // For HeteroCL schedule
    hw_output.compute_root();
    output.compute_root();

    Buffer<uint8_t> out(input.width() - 2, input.height() - 2);
    std::vector<int> output_shape;
    for (int i = 0; i < out.dimensions(); i++){
        output_shape.push_back(out.extent(i));
    }

    output.compile_to_lowered_stmt("median_blur.stmt", {input}, Text);
    std::cout << "HTML Generated\n";
    output.compile_to_heterocl("median_blur_generate.py", {input}, output_shape, "output"); // need some more modification to succefully run Code Generator, but it's doable
    
}