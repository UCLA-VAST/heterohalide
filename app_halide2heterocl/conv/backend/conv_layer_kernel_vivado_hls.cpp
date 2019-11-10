#include <ap_int.h>
#include <ap_fixed.h>
#include <math.h>

void default_function(float _input[4][32][67][67], float _filter[32][32][3][3], float _bias[32], float _f_relu_0[4][32][64][64]) {
  float _f_conv_0[4][32][64][64];
  for (ap_int<32> _f_conv_s0_n = 0; _f_conv_s0_n < 4; ++_f_conv_s0_n) {
    for (ap_int<32> _f_conv_s0_z = 0; _f_conv_s0_z < 32; ++_f_conv_s0_z) {
      for (ap_int<32> _f_conv_s0_y = 0; _f_conv_s0_y < 64; ++_f_conv_s0_y) {
        for (ap_int<32> _f_conv_s0_x = 0; _f_conv_s0_x < 64; ++_f_conv_s0_x) {
          _f_conv_0[_f_conv_s0_n][_f_conv_s0_z][_f_conv_s0_y][_f_conv_s0_x] = _bias[_f_conv_s0_z];
        }
      }
    }
  }
  float _f_conv_1[4][32][64][64];
  for (ap_int<32> _f_conv_s1_n = 0; _f_conv_s1_n < 4; ++_f_conv_s1_n) {
    for (ap_int<32> _f_conv_s1_z = 0; _f_conv_s1_z < 32; ++_f_conv_s1_z) {
      for (ap_int<32> _f_conv_s1_y = 0; _f_conv_s1_y < 64; ++_f_conv_s1_y) {
        for (ap_int<32> _f_conv_s1_x = 0; _f_conv_s1_x < 64; ++_f_conv_s1_x) {
          ap_int<32> reducer0;
          reducer0 = 0;
          for (ap_int<32> ra2 = 0; ra2 < 32; ++ra2) {
            for (ap_int<32> ra1 = 0; ra1 < 3; ++ra1) {
              for (ap_int<32> ra0 = 0; ra0 < 3; ++ra0) {
                reducer0 = ((ap_int<32>)((_filter[_f_conv_s1_z][ra2][ra1][ra0] * _input[_f_conv_s1_n][ra2][(ra1 + _f_conv_s1_y)][(ra0 + _f_conv_s1_x)]) + ((float)reducer0)));
              }
            }
          }
          _f_conv_1[_f_conv_s1_n][_f_conv_s1_z][_f_conv_s1_y][_f_conv_s1_x] = (_f_conv_0[_f_conv_s1_n][_f_conv_s1_z][_f_conv_s1_y][_f_conv_s1_x] + ((float)reducer0));
        }
      }
    }
  }
  for (ap_int<32> _f_relu_s0_n = 0; _f_relu_s0_n < 4; ++_f_relu_s0_n) {
    for (ap_int<32> _f_relu_s0_z = 0; _f_relu_s0_z < 32; ++_f_relu_s0_z) {
      for (ap_int<32> _f_relu_s0_y = 0; _f_relu_s0_y < 64; ++_f_relu_s0_y) {
        for (ap_int<32> _f_relu_s0_x = 0; _f_relu_s0_x < 64; ++_f_relu_s0_x) {
          _f_relu_0[_f_relu_s0_n][_f_relu_s0_z][_f_relu_s0_y][_f_relu_s0_x] = ((0.000000e+00f < _f_conv_1[_f_relu_s0_n][_f_relu_s0_z][_f_relu_s0_y][_f_relu_s0_x]) ? _f_conv_1[_f_relu_s0_n][_f_relu_s0_z][_f_relu_s0_y][_f_relu_s0_x] : 0.000000e+00f);
        }
      }
    }
  }
}