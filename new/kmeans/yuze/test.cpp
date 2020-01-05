#include <cmath>
#include <cstdint>
#include <cstdio>
#ifdef __SSE2__
#include <emmintrin.h>
#elif __ARM_NEON
#include <arm_neon.h>
#endif

#include "HalideBuffer.h"
#include "halide_benchmark.h"

#include "kmeans.h"

using namespace Halide::Runtime;
using namespace Halide::Tools;

#include "halide_blur.h"

int main(int argc, char **argv) {

    Buffer<int32_t> points(N, dim);
    Buffer<int32_t> means(K, dim);
    Buffer<int32_t> labels(N);

    // init
    for (int y = 0; y < points.height(); y++) {
        for (int x = 0; x < points.width(); x++) {
            points(x, y) = rand() % 1000;
        }
    }
    for (int y = 0; y < means.height(); y++) {
        for (int x = 0; x < means.width(); x++) {
            means(x, y) = rand() % 1000;
        }
    }

    // warm up
    halide_blur(points, means, means, labels);

    double t = benchmark(20, niter, [&]() {
      halide_blur(points, means, means, labels);

      /*
      for (int i = 0; i < 20; ++i) {
        printf("%s%d", i != 0 ? ", " : "", labels(i));
      }
      printf("\n");
      */
    });
    printf("%f ms\n", t * niter * 1000);

    return 0;
}
