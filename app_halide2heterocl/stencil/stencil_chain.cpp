#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {

    int stencils = 3; //value, min, max. here iterations = 5
    const int width = 8000; // 1536
    const int height = 12000; // 2560
    Buffer<uint16_t> input(width, height);

    for (int y = 0; y < input.height(); y++){
        for (int x = 0; x < input.width(); x++){
            input(x, y) = rand() % 2; 
        }
    }   

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
        if (argc == 1) {
            f.compute_root();
            f.unroll_hcl(x, 16);
        }
        stages.push_back(f);
    }

    Func output("output");
    output(x, y) = stages.back()(x, y);
    if (argc == 1) {
        output.compute_root();
        output.unroll_hcl(x, 16);
    }

    if (argc == 2) { // CPU schedule
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
    }

    Buffer<uint16_t> out(width - 4 * stencils, height - 4 * stencils);

    const clock_t begin_time = clock();
    output.realize(out);
    float total_time = float( clock() - begin_time ) / CLOCKS_PER_SEC;
    std::ofstream clockfile ("clock.txt", std::ios::app);
    if (clockfile) {
        clockfile << total_time << "\t";
    }

    // std::vector<int> output_shape;
    // for (int i = 0; i < out.dimensions(); i++){
    //     output_shape.push_back(out.extent(i));
    // }

    // output.compile_to_heterocl("stencil_new_generate.py", {input}, output_shape, "output"); // add a parameter to send the output buffer shape into the CodeGen_HeteroCL
    // std::cout << "HeteroCL code Generated" << std::endl;
    // output.compile_to_lowered_stmt("stencil_new.stmt", {input}, Text);


    //     /* ESTIMATES */
    //     // (This can be useful in conjunction with RunGen and benchmarks as well
    //     // as auto-schedule, so we do it in all cases.)
    //     {
    //         const int width = 1536;
    //         const int height = 2560;
    //         // Provide estimates on the input image
    //         input.dim(0).set_bounds_estimate(0, width);
    //         input.dim(1).set_bounds_estimate(0, height);
    //         // Provide estimates on the pipeline output
    //         output.estimate(x, 0, width)
    //               .estimate(y, 0, height);
    //     }

    //     if (auto_schedule) {
    //         // nothing
    //     } else {
    //         // CPU schedule. No fusion.
    //         Var yi, yo, xo, xi, t;
    //         for (size_t i = 1; i < stages.size() - 1; i++) {
    //             Func s = stages[i];
    //             s.store_at(output, t).compute_at(output, yi).vectorize(s.args()[0], 16);
    //         }
    //         output.compute_root()
    //             .tile(x, y, xo, yo, xi, yi, 512, 512)
    //             .fuse(xo, yo, t)
    //             .parallel(t)
    //             .vectorize(xi, 16);
    //     }
    // }

    return 0;
}

