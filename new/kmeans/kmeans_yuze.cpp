#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    constexpr int K = 16;
    constexpr int N = 320;
    constexpr int dim = 32;
    constexpr int niter = 200;

    Buffer<int32_t> points(N, dim);
    Buffer<int32_t> means_in(K, dim);

    Buffer<int32_t> means_out(K, dim);
    Buffer<int32_t> labels(N);

    Func dist("dist");    // N * K
    Var k("k"), n("n"), d("d");
    RDom d_r(0, dim), k_r(0, K);

    Expr dist_ = points(n, d_r) - means_in(k_r, d_r);
    labels(n) = argmin(k_r, sum(d_r, dist_ * dist_))[0];  // N * K * dim additions
    
    // update means
    Func num_k("num_k");  // K
    Func sum_k("sum_k");  // K * dim
    RDom n_r(0, N);

    Expr k_i = clamp(labels(n_r), 0, K - 1);  // N
    num_k(k) = cast<int32_t>(0);
    num_k(k_i) += cast<int32_t>(1);
    sum_k(k, d) = cast<int32_t>(0);
    sum_k(k_i, d) += cast<int32_t>(points(n_r, d));
    // means_out(k, d) = cast<int32_t>(sum_k(k, d)) / cast<int32_t>(max(num_k(k), 1));
    Func final("final");
    final(k, d) = cast<int32_t>(sum_k(k, d)) / cast<int32_t>(max(num_k(k), 1));

    final.compile_to_lowered_stmt("kmeans.stmt", {points, means_in, means_out, labels}, Text);

    return 0;
}

// class HalideBlur : public Halide::Generator<HalideBlur> {
// public:
//     Input<Buffer<int32_t>> points{"points", 2};   // N * dim
//     Input<Buffer<int32_t>> means_in{"means_in", 2};    // K * dim
//     Output<Buffer<int32_t>> means_out{"means_out", 2};    // K * dim
//     Output<Buffer<int32_t>> labels{"labels", 1};  // N

//     void generate() {
//       // update labels
//       Func dist("dist");    // N * K
//       Var k("k"), n("n"), d("d");
//       RDom d_r(0, dim), k_r(0, K);

//       Expr dist_ = points(n, d_r) - means_in(k_r, d_r);
//       labels(n) = argmin(k_r, sum(d_r, dist_ * dist_))[0];  // N * K * dim additions
      
//       // update means
//       Func num_k("num_k");  // K
//       Func sum_k("sum_k");  // K * dim
//       RDom n_r(0, N);

//       Expr k_i = clamp(labels(n_r), 0, K - 1);  // N
//       num_k(k) = cast<int32_t>(0);
//       num_k(k_i) += cast<int32_t>(1);
//       sum_k(k, d) = cast<int32_t>(0);
//       sum_k(k_i, d) += cast<int32_t>(points(n_r, d));
//       means_out(k, d) = cast<int32_t>(sum_k(k, d)) / cast<int32_t>(max(num_k(k), 1));

//       Var ni("ni"), x("x"), xi("xi"), xo("xo");

//       means_out
//         .fuse(k, d, x)
//         .split(x, xo, xi, 64)
//         .parallel(xo)
//         .vectorize(xi, 8)
//         ;
//       num_k
//         .compute_root()
//         ;
//       labels
//         .split(n, n, ni, 80)
//         .parallel(n)
//         .vectorize(ni, 8)
//         ;
//     }
// };

// }  // namespace

// HALIDE_REGISTER_GENERATOR(HalideBlur, halide_blur)
