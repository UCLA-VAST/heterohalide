#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {
    const int width = 6480;
    const int height = 4820;
    Buffer<uint8_t> input(width, height);
    Buffer<uint8_t> filter_x(3, 3);
    Buffer<uint8_t> filter_y(3, 3);

    filter_x(0, 0) = filter_x(2, 0) = -1;
    filter_x(0, 2) = filter_x(2, 2) = 1;
    filter_x(1, 0) = -3; filter_x(1, 2) = 3;

    filter_y(0, 0) = filter_y(0, 2) = -1;
    filter_y(2, 0) = filter_y(2, 2) = 1;
    filter_y(0, 1) = -3; filter_y(2, 1) = 3;
        

    Var x("x"), y("y");
    Func gx("gx"), gy("gy");
    Func output("output");
    RDom r(0, 3, 0, 3);
    gx(x, y) += filter_x(r.x, r.y) * input(x + r.x, y + r.y);
    gy(x, y) += filter_y(r.x, r.y) * gx(x + r.x, y + r.y);
    output(x, y) = gy(x, y);

    
    // CPU schedule
    // gx.compute_at(gy, y)
    //             .vectorize(y, 8);
    // gy.compute_at(output, y)
    //          .vectorize(y, 8);

    // For HeteroCL schedule
    gx.compute_root();
    gy.compute_root();
    output.compute_root();

    Buffer<uint8_t> out(input.width() - 2, input.height() - 2);
    std::vector<int> output_shape;
    for (int i = 0; i < out.dimensions(); i++){
        output_shape.push_back(out.extent(i));
    }

    output.compile_to_lowered_stmt("sobel.stmt", {input, filter_x, filter_y}, Text);
    std::cout << "HTML Generated\n";
    output.compile_to_heterocl("sobel_generate.py", {input, filter_x, filter_y}, output_shape, "output"); // need some more modification to succefully run Code Generator, but it's doable
    
}