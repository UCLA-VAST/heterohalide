#include <string.h>
#include <math.h>
#include <assert.h>
#pragma ACCEL kernel
void default_function(unsigned short* input, unsigned short* final) {
  unsigned short blur_x[308480];
  for (int x = 0; x < 640; ++x) {
    for (int y = 0; y < 482; ++y) {
      blur_x[(y + (x * 482))] = (unsigned short)0;
    }
  }
  int blur_x1;
#pragma ACCEL parallel factor=4 flatten
  for (int blur_x_s0_x = 0; blur_x_s0_x < 640; ++blur_x_s0_x) {
#pragma ACCEL parallel
    for (int blur_x_s0_y = 0; blur_x_s0_y < 482; ++blur_x_s0_y) {
      blur_x[(blur_x_s0_y + (blur_x_s0_x * 482))] = ((unsigned short)((((unsigned int)input[((blur_x_s0_y + (blur_x_s0_x * 482)) + 964)]) + ((unsigned int)(((unsigned int)input[(blur_x_s0_y + (blur_x_s0_x * 482))]) + ((unsigned int)input[((blur_x_s0_y + (blur_x_s0_x * 482)) + 482)])))) / (unsigned int)3));
    }
  }
  unsigned short blur_y[307200];
  for (int x1 = 0; x1 < 640; ++x1) {
    for (int y1 = 0; y1 < 480; ++y1) {
      blur_y[(y1 + (x1 * 480))] = (unsigned short)0;
    }
  }
  int blur_y1;
#pragma ACCEL parallel
  for (int blur_y_s0_y = 0; blur_y_s0_y < 480; ++blur_y_s0_y) {
#pragma ACCEL parallel factor=4 flatten
    for (int blur_y_s0_x = 0; blur_y_s0_x < 640; ++blur_y_s0_x) {
      blur_y[(blur_y_s0_y + (blur_y_s0_x * 480))] = ((unsigned short)((((unsigned int)blur_x[((blur_y_s0_y + (blur_y_s0_x * 482)) + 2)]) + ((unsigned int)(((unsigned int)blur_x[(blur_y_s0_y + (blur_y_s0_x * 482))]) + ((unsigned int)blur_x[((blur_y_s0_y + (blur_y_s0_x * 482)) + 1)])))) / (unsigned int)3));
    }
  }
  for (int x2 = 0; x2 < 640; ++x2) {
    for (int y2 = 0; y2 < 480; ++y2) {
      final[(y2 + (x2 * 480))] = (unsigned short)0;
    }
  }
  int final1;
  for (int final_s0_y = 0; final_s0_y < 480; ++final_s0_y) {
    for (int final_s0_x = 0; final_s0_x < 640; ++final_s0_x) {
      final[(final_s0_y + (final_s0_x * 480))] = blur_y[(final_s0_y + (final_s0_x * 480))];
    }
  }
}

