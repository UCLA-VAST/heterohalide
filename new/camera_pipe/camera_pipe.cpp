#include "Halide.h"
#include "halide_image_io.h"
#include "fstream"
using namespace Halide;

Var x("x"), y("y"), tx("tx"), ty("ty"), c("c"), xi("xi"), yi("yi"), z("z");
Var xo("xo"), yo("yo");
Var x_grid("x_grid"), y_grid("y_grid"), x_in("x_in"), y_in("y_in");
// Func corrected("corrected");
// Func curve("curve");


Expr avg(Expr a, Expr b) {
    Type wider = a.type().with_bits(a.type().bits() * 2);
    return cast(a.type(), (cast(wider, a) + b + 1)/2);
}


Func hot_pixel_suppression(Func input) {
    Expr a = max(max(input(x-2+16, y+12), input(x+2+16, y+12)),
                 max(input(x+16, y-2+12), input(x+16, y+2+12)));
    Expr b = min(min(input(x-2+16, y+12), input(x+2+16, y+12)),
                 min(input(x+16, y-2+12), input(x+16, y+2+12)));

    Func denoised("denoised");
    denoised(x, y) = clamp(input(x+16, y+12), b, a);

    return denoised;
}

Func interleave_x(Func a, Func b) {
    Func out;
    out(x, y) = select((x%2)==0, a(x, y), b(x-1, y));
    return out;
}

Func interleave_y(Func a, Func b) {
    Func out;
    out(x, y) = select((y%2)==0, a(x, y), b(x, y-1));
    return out;
}

// The demosaic algorithm is optimized for HLS schedule
// such that the bound analysis can derive a constant window
// and shift step without needed to unroll 'demosaic' into
// a 2x2 grid.
//
// The chances made from the original is that there is no
// explict downsample and upsample in 'deinterleave' and
// 'interleave', respectively.
// All the intermediate functions are the same size as the
// raw image although only pixels at even coordinates are used.
Func demosaic(Func raw) {

    Func r_r, g_gr, g_gb, b_b;
    g_gr(x, y) = raw(x, y);//deinterleaved(x, y, 0);
    r_r(x, y)  = raw(x+1, y);//deinterleaved(x, y, 1);
    b_b(x, y)  = raw(x, y+1);//deinterleaved(x, y, 2);
    g_gb(x, y) = raw(x+1, y+1);//deinterleaved(x, y, 3);


    // These are the ones we need to interpolate
    Func b_r, g_r, b_gr, r_gr, b_gb, r_gb, r_b, g_b;

    Expr gv_r  = avg(g_gb(x, y-2), g_gb(x, y));
    Expr gvd_r = absd(g_gb(x, y-2), g_gb(x, y));
    Expr gh_r  = avg(g_gr(x+2, y), g_gr(x, y));
    Expr ghd_r = absd(g_gr(x+2, y), g_gr(x, y));

    g_r(x, y)  = select(ghd_r < gvd_r, gh_r, gv_r);

    Expr gv_b  = avg(g_gr(x, y+2), g_gr(x, y));
    Expr gvd_b = absd(g_gr(x, y+2), g_gr(x, y));
    Expr gh_b  = avg(g_gb(x-2, y), g_gb(x, y));
    Expr ghd_b = absd(g_gb(x-2, y), g_gb(x, y));

    g_b(x, y)  = select(ghd_b < gvd_b, gh_b, gv_b);

    Expr correction;
    correction = g_gr(x, y) - avg(g_r(x, y), g_r(x-2, y));
    r_gr(x, y) = correction + avg(r_r(x-2, y), r_r(x, y));

    // Do the same for other reds and blues at green sites
    correction = g_gr(x, y) - avg(g_b(x, y), g_b(x, y-2));
    b_gr(x, y) = correction + avg(b_b(x, y), b_b(x, y-2));

    correction = g_gb(x, y) - avg(g_r(x, y), g_r(x, y+2));
    r_gb(x, y) = correction + avg(r_r(x, y), r_r(x, y+2));

    correction = g_gb(x, y) - avg(g_b(x, y), g_b(x+2, y));
    b_gb(x, y) = correction + avg(b_b(x, y), b_b(x+2, y));

    correction = g_b(x, y)  - avg(g_r(x, y), g_r(x-2, y+2));
    Expr rp_b  = correction + avg(r_r(x, y), r_r(x-2, y+2));
    Expr rpd_b = absd(r_r(x, y), r_r(x-2, y+2));

    correction = g_b(x, y)  - avg(g_r(x-2, y), g_r(x, y+2));
    Expr rn_b  = correction + avg(r_r(x-2, y), r_r(x, y+2));
    Expr rnd_b = absd(r_r(x-2, y), r_r(x, y+2));

    r_b(x, y)  = select(rpd_b < rnd_b, rp_b, rn_b);

    // Same thing for blue at red
    correction = g_r(x, y)  - avg(g_b(x, y), g_b(x+2, y-2));
    Expr bp_r  = correction + avg(b_b(x, y), b_b(x+2, y-2));
    Expr bpd_r = absd(b_b(x, y), b_b(x+2, y-2));

    correction = g_r(x, y)  - avg(g_b(x+2, y), g_b(x, y-2));
    Expr bn_r  = correction + avg(b_b(x+2, y), b_b(x, y-2));
    Expr bnd_r = absd(b_b(x+2, y), b_b(x, y-2));

    b_r(x, y)  =  select(bpd_r < bnd_r, bp_r, bn_r);

    // Interleave the resulting channels
    Func r = interleave_y(interleave_x(r_gr, r_r),
                            interleave_x(r_b, r_gb));
    Func g = interleave_y(interleave_x(g_gr, g_r),
                            interleave_x(g_b, g_gb));
    Func b = interleave_y(interleave_x(b_gr, b_r),
                            interleave_x(b_b, b_gb));


    Func output("demosaiced");
    output(x, y, c) = select(c == 0, r(x, y),
                                c == 1, g(x, y),
                                b(x, y));

    return output;
}


Func color_correct(Func input, int32_t matrix[3][4]) {
    Expr ir = cast<int32_t>(input(x, y, 0));
    Expr ig = cast<int32_t>(input(x, y, 1));
    Expr ib = cast<int32_t>(input(x, y, 2));

    Expr r = matrix[0][3] + matrix[0][0] * ir + matrix[0][1] * ig + matrix[0][2] * ib;
    Expr g = matrix[1][3] + matrix[1][0] * ir + matrix[1][1] * ig + matrix[1][2] * ib;
    Expr b = matrix[2][3] + matrix[2][0] * ir + matrix[2][1] * ig + matrix[2][2] * ib;

    r = cast<int16_t>(r/256);
    g = cast<int16_t>(g/256);
    b = cast<int16_t>(b/256);
    Func corrected("corrected");
    corrected(x, y, c) = select(c == 0, r,
                                select(c == 1, g, b));

    return corrected;
}


Func apply_curve(Func input, float gamma, float contrast) {
    // copied from FCam

    //Expr xf = clamp(cast<float>(x)/1024.0f, 0.0f, 1.0f);
    Expr xf = x/1024.0f;
    Expr g = pow(xf, 1.0f/gamma);
    Expr b = 2.0f - (float) pow(2.0f, contrast/100.0f);
    Expr a = 2.0f - 2.0f*b;
    Expr val = select(g > 0.5f,
                    1.0f - (a*(1.0f-g)*(1.0f-g) + b*(1.0f-g)),
                    a*g*g + b*g);
    Func curve("curve");
    curve(x) = cast<uint8_t>(clamp(val*256.0f, 0.0f, 255.0f));

    Func hw_output("hw_output");
    Expr in_val = clamp(input(x, y, c), 0, 1023);
    hw_output(c, x, y) = select(input(x, y, c) < 0, 0,
                                select(input(x, y, c) >= 1024, 255,
                                        curve(in_val)));

    return hw_output;
}


int main(int argc, char **argv) {
    float color_temp = 3700;
    float gamma = 2.0;
    float contrast = 50;
    
    float matrix_3200[][4] = {{ 1.6697f, -0.2693f, -0.4004f, -42.4346f},
                               {-0.3576f,  1.0615f,  1.5949f, -37.1158f},
                               {-0.2175f, -1.8751f,  6.9640f, -26.6970f}};

    float matrix_7000[][4] = {{ 2.2997f, -0.4478f,  0.1706f, -39.0923f},
                               {-0.3826f,  1.5906f, -0.2080f, -25.4311f},
                               {-0.0888f, -0.7344f,  2.2832f, -20.0826f}};

    int32_t matrix[3][4];
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 4; x++) {
            float alpha = (1.0f/color_temp - 1.0f/3200) / (1.0f/7000 - 1.0f/3200);
            float val =  matrix_3200[y][x] * alpha + matrix_7000[y][x] * (1 - alpha);
            matrix[y][x] = (int32_t)(val * 256.0f); // Q8.8 fixed point
        }
    }

    // int schedule = 2;

    Buffer<uint16_t> input = Tools::load_image(argv[1]);
    std::cout<< "input size: " << input.width() << ", " << input.height()  << "\n"; // 2592, 1968
    


    Func shifted("shifted");
    Func processed("processed");
    Func denoised("denoised");
    // Func deinterleaved("deinterleaved");
    Func demosaiced("demosaiced");
    Func corrected("corrected");
    Func hw_output("hw_output");


    // shifted(x, y) = input(x+16, y+12); // ori
    shifted(x, y) = input(x, y);

    denoised = hot_pixel_suppression(shifted);

    //deinterleaved = deinterleave(denoised);
    demosaiced = demosaic(denoised);
    corrected = color_correct(demosaiced, matrix);
    hw_output = apply_curve(corrected, gamma, contrast);
    
    // processed(x, y, c) = hw_output(c, x, y); // original
    processed(x, y, c) = hw_output(x, y, c);

    shifted.compute_root();
    processed.compute_root();
    denoised.compute_root();
    demosaiced.compute_root();
    corrected.compute_root();
    hw_output.compute_root();
    processed.compute_root();



    Buffer<uint8_t> out(2560, 1920, 3);
    // processed.realize(out);
    // Tools::save_image(output, "out_halide.png");

    std::vector<int> output_shape;
    for (int i = 0; i < out.dimensions(); i++){
        output_shape.push_back(out.extent(i));
    }
    processed.compile_to_lowered_stmt("camera_pipe_new.stmt", {input}, Text);
    
    

    

    return 0;
}