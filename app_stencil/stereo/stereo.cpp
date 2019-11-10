#include "Halide.h"
#include "halide_image_io.h"
#include "fstream"
using namespace Halide;

Var x("x"), y("y"), z("z"), c("c");
Var x_grid("x_grid"), y_grid("y_grid"), xo("xo"), yo("yo"), x_in("x_in"), y_in("y_in");

Func rectify_float(Func img, Func remap) {
    Expr offsetX = cast<float>(cast<int16_t>(remap(x, y, 0)) - cast<int16_t>(128)) / 16.0f;
    Expr offsetY = cast<float>(cast<int16_t>(remap(x, y, 1)) - cast<int16_t>(128)) / 16.0f;

    Expr targetX = cast<int>(floor(offsetX));
    Expr targetY = cast<int>(floor(offsetY));

    Expr wx = offsetX - targetX;
    Expr wy = offsetY - targetY;

    Func interpolated("interpolated");
    interpolated(x, y) = lerp(lerp(img(x+targetX, y+targetY, 1), img(x+targetX+1, y+targetY, 1), wx),
                            lerp(img(x+targetX, y+targetY+1, 1), img(x+targetX+1, y+targetY+1, 1), wx), wy);

    return interpolated;
}

Func rectify_int(Func img, Func remap) {
    Expr targetX = (cast<int16_t>(remap(x, y, 0)) - 128) / 16;
    Expr targetY = (cast<int16_t>(remap(x, y, 1)) - 128) / 16;

    // wx and wy are equal to wx*256 and wy*256 in rectify_float()
    Expr wx = cast<uint8_t>((cast<int16_t>(remap(x, y, 0)) * 16) - (128 * 16) - (targetX * 256));
    Expr wy = cast<uint8_t>((cast<int16_t>(remap(x, y, 1)) * 16) - (128 * 16) - (targetY * 256));

    Func interpolated("interpolated");
    interpolated(x, y) = lerp(lerp(img(x+targetX, y+targetY, 1), img(x+targetX+1, y+targetY, 1), wx),
                            lerp(img(x+targetX, y+targetY+1, 1), img(x+targetX+1, y+targetY+1, 1), wx), wy);

    return interpolated;
}

Func rectify_noop(Func img, Func remap) {
    Func pass("pass");
    pass(x, y) = img(x, y, 1);
    return pass;
}

int main(int argc, char **argv) {
    // Buffer<uint8_t> left = Tools::load_image(argv[1]);
    // Buffer<uint8_t> left_remap = Tools::load_image(argv[2]);
    // Buffer<uint8_t> right = Tools::load_image(argv[3]);
    // Buffer<uint8_t> right_remap = Tools::load_image(argv[4]);

    // std::cout << "left shape: " << left.width() << ", " << left.height() << ", " << left.dim(2).extent() << "\n";
    // std::cout << "left_remap shape: " << left_remap.width() << ", " << left_remap.height() << ", " << left_remap.dim(2).extent() << "\n";
    // std::cout << "right shape: " << right.width() << ", " << right.height() << ", " << right.dim(2).extent() << "\n";
    // std::cout << "right_remap shape: " << right_remap.width() << ", " << right_remap.height() << ", " << right_remap.dim(2).extent() << "\n";
    // shape: 720, 405, 3


    const int width = 720;
    const int height = 405;
    Buffer<uint16_t> left(width, height);
    std::ifstream left_file ("/curr/jiajieli/app_stencil/unsharp/left_halide.txt");
    if (left_file.is_open())
    {   
        for (int y = 0; y < left.height(); y++) { 
            for (int x = 0; x < left.width(); x++){
                left_file >> left(x, y);
            }  
        }   
    }

    Buffer<uint16_t> right(width, height);
    std::ifstream right_file ("/curr/jiajieli/app_stencil/unsharp/right_halide.txt");
    if (right_file.is_open())
    {   
        for (int y = 0; y < right.height(); y++) { 
            for (int x = 0; x < right.width(); x++){
                right_file >> right(x, y);
            }  
        }   
    }

    int windowR = 4;
    int searchR = 64;
    //int windowR = 8;
    //int searchR = 120;



    // Func left_padded("left_padded"), right_padded("right_padded"), left_remap_padded("left_remap_padded"), right_remap_padded("right_remap_padded");
    Func left_remapped("left_remapped"), right_remapped("right_remapped");
    Func SAD("SAD"), offset("offset"), output("output"), hw_output("hw_output");
    // RDom win(-windowR, windowR*2, -windowR, windowR*2);
    RDom win(0, 8, 0, 8);
    RDom search(0, searchR);

    // right_padded = BoundaryConditions::constant_exterior(right, 0);
    // left_padded = BoundaryConditions::constant_exterior(left, 0);
    // right_remap_padded = BoundaryConditions::constant_exterior(right_remap, 128);
    // left_remap_padded = BoundaryConditions::constant_exterior(left_remap, 128);

    
    // std::cout << "left_padded shape: " << left_padded.width() << ", " << left_padded.height() << ", " << left_padded.dim(2).extent() << "\n";
    // std::cout << "left_remap_padded shape: " << left_remap_padded.width() << ", " << left_remap_padded.height() << ", " << left_remap_padded.dim(2).extent() << "\n";
    // std::cout << "right_padded shape: " << right_padded.width() << ", " << right_padded.height() << ", " << right_padded.dim(2).extent() << "\n";
    // std::cout << "right_remap_padded shape: " << right_remap_padded.width() << ", " << right_remap_padded.height() << ", " << right_remap_padded.dim(2).extent() << "\n";

    // right_remapped = rectify_noop(right_padded, right_remap_padded); // not related with remap... simply right_remapped = right_padded[:,:,1]
    // left_remapped = rectify_noop(left_padded, left_remap_padded); // left_remapped = left_padded[:,:,1]

    // right_remapped(x, y) = right_padded(x, y, 1);
    // left_remapped(x, y) = left_padded(x, y, 1);



    SAD(x, y, c) += cast<uint16_t>(absd(right(x + win.x , y + win.y),
                                        left(x + win.x + 20 + c, y + win.y))); // c: [0, 63], because of RDom search use this area

    //offset(x, y) = argmin(SAD(x, y, search.x));
    // avoid using the form of the inlined reduction function of "argmin",
    // so that we can get a handle for scheduling
    offset(x, y) = {cast<int8_t>(0), cast<uint16_t>(65535)};
    offset(x, y) = {select(SAD(x, y, search.x) < offset(x, y)[1],
                            cast<int8_t>(search.x),
                            offset(x, y)[0]),
                    min(SAD(x, y, search.x), offset(x, y)[1])};

    hw_output(x, y) = cast<uint8_t>(cast<uint16_t>(offset(x, y)[0]) * 255 / searchR);
    output(x, y) = hw_output(x, y);

    Buffer<uint8_t> out(left.width() - 91, left.height() - 8);
    std::cout << "out size: " << out.width() << ", " << out.height() << "\n";

    // output.realize(out);
    // Tools::save_image(out, "out_halide.png");

    // right_padded.compute_root();
    // left_padded.compute_root();
    // right_remap_padded.compute_root();
    // left_remap_padded.compute_root();
    // right_remapped.compute_root();
    // left_remapped.compute_root();
    SAD.compute_root();
    offset.compute_root();
    hw_output.compute_root();
    output.compute_root();




    // output.realize(out);
    // std::ofstream outfile ("/curr/jiajieli/app_stencil/unsharp/output_halide.txt");
    // if (outfile)
    // {
    //     for (int c = 0; c < out.dim(2).extent(); c++) {
    //         for (int y = 0; y < out.height(); y++) {
    //             for (int x = 0; x < out.width(); x++) {
    //                 outfile << out(x, y, c) << '\t';            
    //             }
    //         }
    //     }  
    // }

    // Tools::save_image(out, "out_halide.png");




    std::vector<int> output_shape;
    for (int i = 0; i < out.dimensions(); i++){
        output_shape.push_back(out.extent(i));
    }
    output.compile_to_lowered_stmt("stereo.html", {left, right}, HTML);
    std::cout << "HTML generated\n";
    output.compile_to_heterocl("stereo_generate.py", {left, right}, output_shape, "output");

    return 0;
}