// very weird...: if Buffer type is uint8_t instead of uin16_t, it can't be read/write using fstream correctly. But uint16_t is fine......
#include "Halide.h"
#include "halide_image_io.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {


    ImageParam input(type_of<uint8_t>(), 2);


    Var x("x"), y("y"), c("c");
    Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

    int blockSize = 3;
    int Ksize = 3;

    float k = 0.04;
    float threshold = 100;

    Func padded("padded");
    Func grad_x("grad_x"), grad_y("grad_y");
    Func grad_xx("grad_xx"), grad_yy("grad_yy"), grad_xy("grad_xy");
    Func grad_gx("grad_gx"), grad_gy("grad_gy"), grad_gxy("grad_gxy");
    Func cim("cim");
    Func output("output"), hw_output("hw_output");
    RDom box(0, 3, 0, 3);

    //padded = BoundaryConditions::repeat_edge(input);
    // padded(x, y) = input(x+3, y+3);

    // sobel filter
    Func padded16;
    padded16(x,y) = cast<int16_t>(input(x,y));
    grad_x(x, y) = cast<int16_t>(-padded16(x,y) + padded16(x+2,y)
                                    -2*padded16(x,y+1) + 2*padded16(x+2,y+1)
                                    -padded16(x,y+2) + padded16(x+2,y+2));
    grad_y(x, y) = cast<int16_t>(padded16(x,y+2) - padded16(x,y) +
                                    2*padded16(x+1,y+2) - 2*padded16(x+1,y) +
                                    padded16(x+2,y+2) - padded16(x+2,y));

    grad_xx(x, y) = cast<int32_t>(grad_x(x,y)) * cast<int32_t>(grad_x(x,y));
    grad_yy(x, y) = cast<int32_t>(grad_y(x,y)) * cast<int32_t>(grad_y(x,y));
    grad_xy(x, y) = cast<int32_t>(grad_x(x,y)) * cast<int32_t>(grad_y(x,y));

    // box filter (i.e. windowed sum)
    grad_gx(x, y) += grad_xx(x+box.x, y+box.y);
    grad_gy(x, y) += grad_yy(x+box.x, y+box.y);
    grad_gxy(x, y) += grad_xy(x+box.x, y+box.y);

    // calculate Cim
    int scale = (1 << (Ksize-1)) * blockSize; // original: int
    // std::cout << "scale: " << scale << "\n";
    Expr lgx = cast<float>(grad_gx(x, y) / scale / scale); // some modification, but seems didnt change anything. 
    Expr lgy = cast<float>(grad_gy(x, y) / scale / scale);
    Expr lgxy = cast<float>(grad_gxy(x, y) / scale / scale);
    Expr det = lgx*lgy - lgxy*lgxy;
    Expr trace = lgx + lgy;
    cim(x, y) = det - k*trace*trace;
    // cim(x, y) = cast<float>(grad_gx(x, y)) / scale / scale * cast<float>(grad_gy(x, y)) / scale / scale - cast<float>(grad_gxy(x, y)) / scale / scale * cast<float>(grad_gxy(x, y)) / scale / scale - k * (cast<float>(grad_gx(x, y)) / scale / scale + cast<float>(grad_gy(x, y)) / scale / scale) * (cast<float>(grad_gx(x, y)) / scale / scale + cast<float>(grad_gy(x, y)) / scale / scale);

    // Perform non-maximal suppression
    Expr is_max = cim(x+1, y+1) > cim(x, y) && cim(x+1, y+1) > cim(x+1, y) &&
        cim(x+1, y+1) > cim(x+1, y) && cim(x+1, y+1) > cim(x, y+1) &&
        cim(x+1, y+1) > cim(x+2, y+1) && cim(x+1, y+1) > cim(x, y+2) &&
        cim(x+1, y+1) > cim(x+1, y+2) && cim(x+1, y+1) > cim(x+2, y+2);
    hw_output(x, y) = select( is_max && (cim(x+1, y+1) >= threshold), cast<uint8_t>(255), 0);

    output(x, y) = hw_output(x, y);

    // compute_root
    // if (argc == 1) {
    //     padded.compute_root();
    //     padded16.compute_root();
    //     grad_x.compute_root();
    //     grad_y.compute_root();
    //     grad_xx.compute_root();
    //     grad_yy.compute_root();
    //     grad_xy.compute_root();
    //     grad_gx.compute_root();
    //     grad_gy.compute_root();
    //     grad_gxy.compute_root();
    //     cim.compute_root();
    //     hw_output.compute_root();
    //     output.compute_root();
    // }


    output.tile(x, y, xo, yo, xi, yi, 240, 320);
    grad_x.compute_at(output, xo).vectorize(x, 8);
    grad_y.compute_at(output, xo).vectorize(x, 8);
    grad_xx.compute_at(output, xo).vectorize(x, 4);
    grad_yy.compute_at(output, xo).vectorize(x, 4);
    grad_xy.compute_at(output, xo).vectorize(x, 4);
    grad_gx.compute_at(output, xo).vectorize(x, 4);
    grad_gy.compute_at(output, xo).vectorize(x, 4);
    grad_gxy.compute_at(output, xo).vectorize(x, 4);
    cim.compute_at(output, xo).vectorize(x, 4);

    grad_gx.update(0).unroll(box.x).unroll(box.y);
    grad_gy.update(0).unroll(box.x).unroll(box.y);
    grad_gxy.update(0).unroll(box.x).unroll(box.y);

    output.fuse(xo, yo, xo).parallel(xo).vectorize(xi, 4);
    
    output.compile_to_static_library("harris_lib", {input}, "harris");
}