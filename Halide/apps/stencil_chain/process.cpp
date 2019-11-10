#include <cstdio>
#include <chrono>

#include "stencil_chain.h"
#ifndef NO_AUTO_SCHEDULE
#include "stencil_chain_auto_schedule.h"
#endif

#include "halide_benchmark.h"
#include "HalideBuffer.h"
#include "halide_image_io.h"

#include<iostream>
#include<fstream>

using namespace Halide::Runtime;
using namespace Halide::Tools;

using namespace std;

int main(int argc, char **argv) {
    // if (argc < 4) {
    //     printf("Usage: ./process input.png timing_iterations output.png\n"
    //            "e.g.: ./process input.png 10 output.png\n");
    //     return 0;
    // }

    // // original:
    // // Input may be a PNG8
    // Buffer<uint16_t> input = load_and_convert_image(argv[1]); // don't know how this function works... PNG is uint8 format. original: uint16_t
    // // Just take the red channel
    // input.slice(2, 0);

    Buffer<uint16_t> input(1536, 2560);
    cout << input.width() << '\t' << input.height() << '\t' << endl;

    Buffer<uint16_t> output(input.width(), input.height()); // original uint16_t
    // int timing = atoi(argv[2]);

    // read input from input_halide.txt generated from input_generator.py
    ifstream input_file ("/curr/jiajieli/app_halide2heterocl/stencil/input_halide.txt");   
    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++) {
            input_file >> input(x, y);
            // cout << input(x,y) << '\t';

            // if (y == 0 && x < 50){
            //     cout << input(x, y) << '\t'; //very big number... lots of 10000+, some are 1000,000,000 ......
            // // cout << filter(x,y,z,c) << '\t';
            // }
        }
        // cout << '\n';
    }

    stencil_chain(input, output);

    cout << "width: " << output.width() << endl << "height: " << output.height() << endl;
    // cout << output.dim(2).extent() << endl; // no dim(2)

    ofstream output_file ("/curr/jiajieli/app_halide2heterocl/stencil/output_halide_1.txt");
    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
            output_file << output(x, y) << '\t';

            // if (y == 0 && x < 50){
            //     cout << input(x, y) << '\t'; //very big number... lots of 10000+, some are 1000,000,000 ......
            // cout << filter(x,y,z,c) << '\t';
        }
        // cout << '\n';
    }

    
    // Timing code

    // Manually-tuned version
    double best_manual = benchmark(10, 1, [&]() { //original: timing = 10, don't know what timing means?
        stencil_chain(input, output);
    });
    printf("Manually-tuned time: %gms\n", best_manual * 1e3);

    #ifndef NO_AUTO_SCHEDULE
    // Auto-scheduled version
    double best_auto = benchmark(10, 1, [&]() { //original: timing = 10, don't know what timing means?
        stencil_chain_auto_schedule(input, output);
    });
    printf("Auto-scheduled time: %gms\n", best_auto * 1e3);
    #endif

    // convert_and_save_image(output, argv[3]);

    return 0;
}
