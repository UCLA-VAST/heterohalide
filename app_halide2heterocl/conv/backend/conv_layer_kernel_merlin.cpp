#include <string.h>
#include <math.h>
#include <assert.h>
#pragma ACCEL kernel
void default_function(float* _input, float* _filter, float* _bias, float* _f_relu_0) {
  #pragma ACCEL interface variable=_input depth=(4*32*67*67)
  #pragma ACCEL interface variable=_filter depth=(32*32*3*3)
  #pragma ACCEL interface variable=_bias depth=32
  #pragma ACCEL interface variable=_f_relu_0 depth=(4*32*64*64)

  float _f_conv_0[524288];
  for (int _f_conv_s0_n = 0; _f_conv_s0_n < 4; ++_f_conv_s0_n) {
    for (int _f_conv_s0_z = 0; _f_conv_s0_z < 32; ++_f_conv_s0_z) {
      for (int _f_conv_s0_y = 0; _f_conv_s0_y < 64; ++_f_conv_s0_y) {
        for (int _f_conv_s0_x = 0; _f_conv_s0_x < 64; ++_f_conv_s0_x) {
          _f_conv_0[(((_f_conv_s0_x + (_f_conv_s0_y * 64)) + (_f_conv_s0_z * 4096)) + (_f_conv_s0_n * 131072))] = _bias[_f_conv_s0_z];
        }
      }
    }
  }
  float _f_conv_1[524288];
  for (int _f_conv_s1_n = 0; _f_conv_s1_n < 4; ++_f_conv_s1_n) {
    for (int _f_conv_s1_z = 0; _f_conv_s1_z < 32; ++_f_conv_s1_z) {
      for (int _f_conv_s1_y = 0; _f_conv_s1_y < 64; ++_f_conv_s1_y) {
        for (int _f_conv_s1_x = 0; _f_conv_s1_x < 64; ++_f_conv_s1_x) {
          int reducer0;
          reducer0 = 0;
          for (int ra2 = 0; ra2 < 32; ++ra2) {
            for (int ra1 = 0; ra1 < 3; ++ra1) {
              for (int ra0 = 0; ra0 < 3; ++ra0) {
                reducer0 = ((int)((_filter[(((ra0 + (ra1 * 3)) + (ra2 * 9)) + (_f_conv_s1_z * 288))] * _input[((((ra0 + _f_conv_s1_x) + ((ra1 + _f_conv_s1_y) * 67)) + (ra2 * 4489)) + (_f_conv_s1_n * 143648))]) + ((float)reducer0)));
              }
            }
          }
          _f_conv_1[(((_f_conv_s1_x + (_f_conv_s1_y * 64)) + (_f_conv_s1_z * 4096)) + (_f_conv_s1_n * 131072))] = (_f_conv_0[(((_f_conv_s1_x + (_f_conv_s1_y * 64)) + (_f_conv_s1_z * 4096)) + (_f_conv_s1_n * 131072))] + ((float)reducer0));
        }
      }
    }
  }
  for (int _f_relu_s0_n = 0; _f_relu_s0_n < 4; ++_f_relu_s0_n) {
    for (int _f_relu_s0_z = 0; _f_relu_s0_z < 32; ++_f_relu_s0_z) {
      for (int _f_relu_s0_y = 0; _f_relu_s0_y < 64; ++_f_relu_s0_y) {
        for (int _f_relu_s0_x = 0; _f_relu_s0_x < 64; ++_f_relu_s0_x) {
          _f_relu_0[(((_f_relu_s0_x + (_f_relu_s0_y * 64)) + (_f_relu_s0_z * 4096)) + (_f_relu_s0_n * 131072))] = ((0.000000e+00f < _f_conv_1[(((_f_relu_s0_x + (_f_relu_s0_y * 64)) + (_f_relu_s0_z * 4096)) + (_f_relu_s0_n * 131072))]) ? _f_conv_1[(((_f_relu_s0_x + (_f_relu_s0_y * 64)) + (_f_relu_s0_z * 4096)) + (_f_relu_s0_n * 131072))] : 0.000000e+00f);
        }
      }
    }
  }
}