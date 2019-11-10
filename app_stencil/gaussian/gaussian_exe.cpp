#include "gaussian_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<uint8_t> input(1500, 2500); // 2160, 3840
    Halide::Runtime::Buffer<uint8_t> output(input.width() - 8, input.height() - 8);

    auto begin = std::chrono::high_resolution_clock::now();
    uint32_t iterations = 100;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = gaussian(input, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;

    return 0;
}