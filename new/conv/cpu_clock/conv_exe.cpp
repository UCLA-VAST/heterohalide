#include "conv_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<float> input(67, 67, 32, 4); 
    Halide::Runtime::Buffer<float> filter(3, 3, 32, 32); 
    Halide::Runtime::Buffer<float> bias(32); 
    Halide::Runtime::Buffer<float> output(64, 64, 32, 4);

    // const clock_t begin_time = clock();
    // int error = conv(input, output);
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
    uint32_t iterations = 100;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = conv(input, filter, bias, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;



    return 0;
}