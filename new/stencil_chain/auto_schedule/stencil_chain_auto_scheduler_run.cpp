// Halide tutorial lesson 21: Auto-Scheduler

// Before reading this file, see lesson_21_auto_scheduler_generate.cpp

// This is the code that actually uses the Halide pipeline we've
// compiled. It does not depend on libHalide, so we won't be including
// Halide.h.
//
// Instead, it depends on the header files that lesson_21_auto_scheduler_generator produced.
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
    Halide::Runtime::Buffer<uint16_t> input(1536, 2560); 

    for (int y = 0; y < input.height(); ++y) {
        for (int x = 0; x < input.width(); ++x) {
            input(x, y) = rand();
        }
    }

    Halide::Runtime::Buffer<uint16_t> output(input.width() - 12, input.height() - 12);
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