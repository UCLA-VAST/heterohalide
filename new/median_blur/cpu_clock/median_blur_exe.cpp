#include "median_blur_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<uint8_t> input(6480, 4820); 
    Halide::Runtime::Buffer<uint8_t> output(input.width() - 2, input.height() - 2);

    // const clock_t begin_time = clock();
    // int error = median_blur(input, output);
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
        int error = median_blur(input, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;



    return 0;
}