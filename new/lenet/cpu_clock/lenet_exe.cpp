#include "lenet_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {

    Halide::Runtime::Buffer<int16_t> input(28, 28, 1, 500); // width, height, channels, batch
    Halide::Runtime::Buffer<int16_t> weight_conv1(5, 5, 1, 20); // kernel_w, kernel_h, input_channel, output_channel
    Halide::Runtime::Buffer<int16_t> weight_conv2(5, 5, 20, 50); 
    Halide::Runtime::Buffer<int16_t> weight_fc1(500, 800);
    Halide::Runtime::Buffer<int16_t> weight_fc2(10, 500);
    
    Halide::Runtime::Buffer<float> output(10, 500);

    // const clock_t begin_time = clock();
    // int error = lenet(input, output);
    // float total_time = float( clock() - begin_time ) / CLOCKS_PER_SEC;

    // if (error) {
    //     printf("Halide returned an error\n");
    //     return -1;
    // }

    // std::ofstream clockfile ("clock.txt", std::ios::app);
    // if (clockfile) {
    //     clockfile << total_time << "\t";
    // }

    auto begin = std::chrono::high_resolution_clock::now();
    uint32_t iterations = 1;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = lenet(input, weight_conv1, weight_conv2, weight_fc1, weight_fc2, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;



    return 0;
}