#include "harris_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<uint8_t> input(1200, 1700); // 2448, 3264
    Halide::Runtime::Buffer<uint8_t> output(input.width() - 6, input.height() - 6);

    auto begin = std::chrono::high_resolution_clock::now();
    uint32_t iterations = 100;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = harris(input, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;

    return 0;
}