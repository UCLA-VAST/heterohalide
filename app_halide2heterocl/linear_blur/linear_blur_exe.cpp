#include "linear_blur_lib.h"
#include "HalideBuffer.h"
#include "fstream"
#include <iostream>
#include <chrono>
int main(int argc, char **argv) {
    Halide::Runtime::Buffer<float> input(768, 1280, 3); // 768, 1280, 3
    Halide::Runtime::Buffer<float> output(input.width() - 2, input.height() - 2, 3);

    auto begin = std::chrono::high_resolution_clock::now();
    uint32_t iterations = 100;
    for(uint32_t i = 0; i < iterations; ++i)
    {
        int error = linear_blur(input, output);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / iterations << "ns." << std::endl;

    return 0;
}