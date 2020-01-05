#include "sobel_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<int16_t> input(6480, 4820); 
    Halide::Runtime::Buffer<int16_t> filter_x(3, 3);
    Halide::Runtime::Buffer<int16_t> filter_y(3, 3);
    Halide::Runtime::Buffer<int16_t> output(input.width() - 4, input.height() - 4);

    auto begin = std::chrono::high_resolution_clock::now();
    uint32_t iterations = 10;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = sobel(input, filter_x, filter_y, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;

    return 0;
}