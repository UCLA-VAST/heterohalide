#include "Halide.h"
#include "fstream"
using namespace Halide;

int main(int argc, char **argv) {
    Buffer<int16_t> A(1024, 1024);
    Buffer<int16_t> B(1024, 1024);

    // std::ifstream A_file ("/curr/jiajieli/new/gemm/A.txt");
    // for (int y = 0; y < A.height(); y++) {
    //     for (int x = 0; x < A.width(); x++) {
    //         A_file >> A(x, y);
    //     }
    // }
        
    // std::ifstream B_file ("/curr/jiajieli/new/gemm/B.txt");
    // for (int y = 0; y < B.height(); y++) {
    //     for (int x = 0; x < B.width(); x++) {
    //         B_file >> B(x, y);
    //     }
    // }

    for (int y = 0; y < A.height(); y++) {
        for (int x = 0; x < A.width(); x++) {
            A(x, y) = rand() % 10;
        }
    }
    
    for (int y = 0; y < B.height(); y++) {
        for (int x = 0; x < B.width(); x++) {
            B(x, y) = rand() % 10;
        }
    }


    Var x{"x"}, y{"y"};
    Func prod("prod");
    RDom r(0, A.height());
    prod(x, y) += A(x, r) * B(r, y);

    Func final("final");
    final(x, y) = prod(x, y);


    // prod.compute_root();
    final.compute_root();

    Var xi{"xi"}, yi{"yi"};
    prod.update()
        .tile(x, y, xi, yi, 8, 8)
        .reorder(x, y, xi, yi);
    

    Buffer<float> output(1024, 1024);


    std::vector<int> output_shape;
    for (int i = 0; i < output.dimensions(); i++){
        output_shape.push_back(output.extent(i));
    }    

    final.compile_to_lowered_stmt("gemm_tile.stmt", {A, B}, Text);
    final.compile_to_heterocl("gemm_tile.py", {A, B}, output_shape, "final");

    return 0;
}

// heterocl IR

// produce out_matrix {
//   // attr [0] extern_scope = 0
//   for (x.outer, 0, 128) {
//     for (y.outer, 0, 128) {
//       for (x.inner, 0, 8) {
//         for (y.inner, 0, 8) {
//           // attr [sum] storage_scope = "global"
//           allocate sum[float32 * 1]
//           produce sum {
//             // attr [0] extern_scope = 0
//             sum[0] = 0.000000f
//           }
//           for (k, 0, 1024) {
//             sum[0] = ((placeholder0[(k + ((x.inner + (x.outer*8))*1024))]*placeholder1[((y.inner + (y.outer*8)) + (k*1024))]) + sum[0])
//           }
//           out_matrix[((y.inner + (y.outer*8)) + ((x.inner + (x.outer*8))*1024))] = sum[0]
//         }
//       }
//     }
//   }
// }