#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "linear_blur.h"
#include "simple_blur.h"

#include "HalideBuffer.h"
#include "halide_image_io.h"

#include <iostream>
#include <fstream>

using namespace Halide::Tools;
using namespace Halide::Runtime;

using namespace std;

int main(int argc, char **argv) {
    // if (argc != 4) {
    //     printf("Usage: ./linear_blur use_linear input.png output.png\n");
    //     return 0;
    // }

    // int use_linear = atoi(argv[1]);

    // Buffer<float> input = load_and_convert_image(argv[2]);
    // Buffer<float> output = Buffer<float>::make_with_shape_of(input);

    // // Call either the simple or linear-corrected blur at runtime,
    // // mainly to demonstrate how simple_blur can be used either standalone
    // // or fused into another Generator.
    // if (use_linear) {
    //     printf("Using linear blur...\n");
    //     linear_blur(input, output);
    // } else {
    //     printf("Using simple blur...\n");
    //     simple_blur(input, input.width(), input.height(), output);
    // }

    // convert_and_save_image(output, argv[3]);

    Buffer<float> input(768, 1280, 3);
    Buffer<float> output(768, 1280, 3);
    
    std::ifstream input_file ("/curr/jiajieli/app_halide2heterocl/linear_blur/input.txt");

    for (int z = 0; z < input.channels(); z++) {
        for (int y = 0; y < input.height(); y++) {
            for (int x = 0; x < input.width(); x++) {
                input_file >> input(x, y, z);
            }
        }
    }

    linear_blur(input, output);

    printf("input height: %d\n", input.height());
    printf("input width: %d\n", input.width());

    // for (int c = 0; c < input.dim(2).extent(); c++){
    //     for (int y = 0; y < input.height(); y++) {
    //         for (int x = 0; x < input.width(); x++) {
    //             // We can access a pixel of an Buffer object using similar
    //             // syntax to defining and using functions.
    //             // if (input(x,y,c) != input(x,y,c)) {
    //             //     printf("input is not equal to Input!\n"
    //             //             "Pixel %d, %d, %d\n",
    //             //             x, y, c);
    //             //     return -1;
    //             printf("%f\t", input(x,y,c));
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }

    // ofstream outfile ("/curr/jiajieli/app_halide2heterocl/linear_blur/output.txt");
    // for (int c = 2; c >= 0; c--){
    //     for (int y = 0; y < output.height(); y++) {
    //         for (int x = 0; x < output.width(); x++) {
    //             // We can access a pixel of an Buffer object using similar
    //             // syntax to defining and using functions.
    //             // if (output(x,y,c) != input(x,y,c)) {
    //             //     printf("Output is not equal to Input!\n"
    //             //             "Pixel %d, %d, %d\n",
    //             //             x, y, c);
    //             //     return -1;
    //             outfile << output(x,y,c) << '\t';
    //         }
    //         outfile << '\n';
    //     }
    //     outfile << '\n';
    // }

    std::cout << "original halide program: \n";
    std::ofstream output_file ("/curr/jiajieli/app_halide2heterocl/linear_blur/output_halide_ori.txt");

    for (int z = 0; z < output.channels(); z++) {
        for (int y = 0; y < output.height(); y++) {
            for (int x = 0; x < output.width(); x++) {
                output_file << output(x, y, z) << '\t';
            }
        }
    }


    



    printf("output height: %d\n", output.height());
    printf("output width: %d\n", output.width());

    return 0;
}
