#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    int k = 16;
    int width = 320; int height = 32;
    Func clustersFunc("clustersFunc"), kmeans("kmeans");
    Buffer<uint8_t> clusters({k});
    Buffer<uint8_t> input((uint8_t*)src, width * height);
    Func clustersMap("clustersMap");
    Var x("x");
    RDom r(0, k);
    clustersMap(x) = argmin(abs(cast<int16_t>(input(x)) - cast<int16_t>(clusters(r))))[0];

    Var i("i");
    RDom im(input);
    Func count("count");
    count(i) = 0;
    Expr clusterId = clamp(clustersMap(im), 0, k - 1);
    count(clusterId) += 1;
    Func s("s");
    s(i) = 0;
    s(clusterId) += cast<uint32_t>(input(im));

    clustersFunc(i) = cast<uint8_t>(s(i) / max(count(i), 1));
    // clustersFunc.estimate(i, 0, k);
    kmeans(x) = clustersFunc(clamp(clustersMap(x), 0, k - 1));


    // kmeans.estimate(x, 0, width * height);

    // Pipeline({clustersFunc, kmeans}).auto_schedule(get_host_target());
    // for (int i = 0; i < k; ++i)
    //     clusters(i) = rand() % 256;
    // for (int i = 0; i < 14; ++i)
    //     clustersFunc.realize(clusters);

    // Buffer<uint8_t> output((uint8_t*)dst, width * height);

    // kmeans.realize(output);

    Buffer<uint8_t> output((uint8_t*)dst, width * height);


    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }    

    kmeans.compile_to_lowered_stmt("kmeans.stmt", {clusters, input}, Text);
    // kmeans.compile_to_heterocl("gemm_tile.py", {A, B}, output_shape, "final");

    return 0;
}