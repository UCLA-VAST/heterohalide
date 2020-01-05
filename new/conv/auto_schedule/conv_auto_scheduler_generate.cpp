#include "Halide.h"
#include <stdio.h>

using namespace Halide;

// We will define a generator to auto-schedule.
class AutoScheduled : public Halide::Generator<AutoScheduled> {
public:
    Input<Buffer<float>> input{"input", 4};
    Input<Buffer<float>> filter{"filter", 4};
    Input<Buffer<float>> bias{"bias", 1};

    Output<Buffer<float>> f_ReLU{"ReLU", 4};

    void generate() {

        f_conv(x, y, z, n) = bias(z);

        f_conv(x, y, z, n) += filter(r.x, r.y, r.z, z) * input(x + r.x, y + r.y, r.z, n);

        f_ReLU(x, y, z, n) = max(0, f_conv(x, y, z, n));
    }

    void schedule() {
        if (auto_schedule) {
            // The auto-scheduler requires estimates on all the input/output
            // sizes and parameter values in order to compare different
            // alternatives and decide on a good schedule.

            // To provide estimates (min and extent values) for each dimension
            // of the input images ('input', 'filter', and 'bias'), we use the
            // set_bounds_estimate() method. set_bounds_estimate() takes in
            // (min, extent) of the corresponding dimension as arguments.
            // input.set_estimates({{0, 131}, {0, 131}, {0, 64}, {0, 4}});
            // filter.set_estimates({{0, 3}, {0, 3}, {0, 64}, {0, 64}});
            // bias.set_estimates({{0, 64}});

            input.dim(0).set_bounds_estimate(0, 131);
            input.dim(1).set_bounds_estimate(0, 131);
            input.dim(2).set_bounds_estimate(0, 64);
            input.dim(3).set_bounds_estimate(0, 4);
            
            filter.dim(0).set_bounds_estimate(0, 3);
            filter.dim(1).set_bounds_estimate(0, 3);
            filter.dim(2).set_bounds_estimate(0, 64);
            filter.dim(3).set_bounds_estimate(0, 64);

            bias.dim(0).set_bounds_estimate(0, 64);
            


            // f_ReLU.set_estimates({{0, 128}, {0, 128}, {0, 64}, {0, 4}});
            f_ReLU.estimate(x, 0, 128)
                    .estimate(y, 0, 128)
                    .estimate(z, 0, 64)
                    .estimate(n, 0, 4);




        } else {
            // Var z_t{"z_t"}, y_t{"y_t"}, par{"par"};
            // int vec_len = 8;
            // int o_block_size = 32;
            // int y_block = 32;
            // f_conv.compute_root();
            // f_conv.fuse(z, n, par).parallel(par);
            // f_conv.update()
            //     .reorder(x, y, r.z)
            //     .split(y, y, y_t, y_block)
            //     .split(z, z, z_t, o_block_size)
            //     .reorder(y_t, z_t, y, r.z, z)
            //     .vectorize(x, vec_len)
            //     .unroll(r.x, 3)
            //     .unroll(r.y, 3)
            //     .fuse(z, n, par)
            //     .parallel(par);
            // f_ReLU.reorder(n, z).parallel(z).vectorize(x, 8);
        }
    }
private:
    Var x{"x"}, y{"y"}, z{"z"}, n{"n"};
    Func f_conv{"conv"};
    RDom r{filter.dim(0).min(), filter.dim(0).extent(),
               filter.dim(1).min(), filter.dim(1).extent(),
               filter.dim(2).min(), filter.dim(2).extent()};
};

// As in lesson 15, we register our generator and then compile this
// file along with tools/GenGen.cpp.
HALIDE_REGISTER_GENERATOR(AutoScheduled, auto_schedule_gen)

// After compiling this file, see how to use it in
