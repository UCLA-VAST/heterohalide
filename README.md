# HeteroHalide
From Image Processing DSL to Efficient FPGA Acceleration

## Publication
Jiajie Li, Yuze Chi, Jason Cong. HeteroHalide: From Image Processing DSL to Efficient FPGA Acceleration. In *FPGA*, 2020. [[PDF]](https://vast.cs.ucla.edu/~chiyuze/pub/fpga20-heterohalide.pdf) [[Slides]](https://vast.cs.ucla.edu/~chiyuze/pub/fpga20-heterohalide.slides.pdf)

## Evaluation: Comparison with CPU
| Benchmark | VU9P (AWS F1) Energy Eff | VU9P Speedup | Stratix 10 MX Energy Eff | Stratix 10 MX Speedup | Pattern (Backend) | 
| --- | --- | --- | --- | --- | --- |
| Harris | 29.11 | 10.31 | 12.36 | 9.89 | Stencil (SODA) | 
| Gaussian | 17.17 | 6.08 | - | - | Stencil (SODA) | 
| Unsharp | 9.57 | 3.39 | - | - | Stencil (SODA) | 
| Blur | 10.98 | 3.89 | - | - | Stencil (SODA) | 
| Linear Blur | 12.65 | 4.48 | 10.75 | 8.60 | Stencil (SODA) |
| Stencil Chain | 4.29 | 1.52 | 3.64 | 2.91 | Stencil (SODA) | 
| Dilation | 4.69 | 1.66 | 1.99 | 1.59 | Stencil (SODA) |
| Median Blur | 12.51 | 4.43 | 5.30 | 4.24 | Stencil (SODA) | 
| GEMM | 9.97 | 3.53 | - | - | Systolic Array (PolySA) | 
| K-Means | 29.00 | 10.27 | - | - | General (Merlin Compiler) | 
| Geo.Mean | 11.71 | 4.15 | 6.02 | 4.82 | - |

### Configurations
CPU: Xeon 2680v4 x2, 28 cores, 14nm, f=2.4GHz, TDP=240W
Xilinx Vu9P: 16nm, f=250MHz, TDP=85W
S10MX: 14nm, f=480MHz, TDP=192W