#include "auto_schedule_false.h"
#include "auto_schedule_true.h"

// We'll use the Halide::Runtime::Buffer class for passing data into and out of
// the pipeline.
#include "HalideBuffer.h"
#include "halide_benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char **argv) {
    // Let's declare and initialize the input images
    Halide::Runtime::Buffer<int16_t> A(1024, 1024);
    Halide::Runtime::Buffer<int16_t> B(1024, 1024);

    for (int y = 0; y < input.height(); ++y) {
        for (int x = 0; x < input.width(); ++x) {
            input(x, y) = rand();
        }
    }

    Halide::Runtime::Buffer<int16_t> prod(1024, 1024);
    // Run each version of the codes (with no auto-schedule and with
    // auto-schedule) multiple times for benchmarking.
    double auto_schedule_off = Halide::Tools::benchmark(2, 5, [&]() {
        auto_schedule_false(input, output);
    });
    printf("Manual schedule: %gms\n", auto_schedule_off * 1e3);

    double auto_schedule_on = Halide::Tools::benchmark(2, 5, [&]() {
        auto_schedule_true(input, output);
    });
    printf("Auto schedule: %gms\n", auto_schedule_on * 1e3);

    // auto_schedule_on should be faster since in the auto_schedule_off version,
    // the schedule is very simple.
    assert(auto_schedule_on < auto_schedule_off);

    return 0;
}
