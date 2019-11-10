#include "stencil_chain_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<uint16_t> input(1300, 2000); // 1536, 2560
    Halide::Runtime::Buffer<uint16_t> output(input.width() - 4 * 3, input.height() - 4 * 3);

    auto begin = std::chrono::high_resolution_clock::now();
    uint32_t iterations = 10000;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = stencil_chain(input, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;

    return 0;
}