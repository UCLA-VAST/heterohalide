#include <string.h>
#include <math.h>
#include <assert.h>
#pragma ACCEL kernel
void default_function(unsigned short* input, unsigned short* blur_y_0) {
  unsigned short blur_x_0[31195040];
  for (int blur_x_s0_y = 0; blur_x_s0_y < 4820; ++blur_x_s0_y) {
#pragma ACCEL parallel factor=4 flatten
    for (int blur_x_s0_x = 0; blur_x_s0_x < 6472; ++blur_x_s0_x) {
      blur_x_0[(blur_x_s0_x + (blur_x_s0_y * 6472))] = ((unsigned short)((((unsigned int)input[((blur_x_s0_x + (blur_x_s0_y * 6480)) + 2)]) + ((unsigned int)(((unsigned int)input[(blur_x_s0_x + (blur_x_s0_y * 6480))]) + ((unsigned int)input[((blur_x_s0_x + (blur_x_s0_y * 6480)) + 1)])))) / (unsigned int)3));
    }
  }
  for (int blur_y_s0_y = 0; blur_y_s0_y < 4818; ++blur_y_s0_y) {
#pragma ACCEL parallel factor=4 flatten
    for (int blur_y_s0_x = 0; blur_y_s0_x < 6472; ++blur_y_s0_x) {
      blur_y_0[(blur_y_s0_x + (blur_y_s0_y * 6472))] = ((unsigned short)((((unsigned int)blur_x_0[((blur_y_s0_x + (blur_y_s0_y * 6472)) + 12944)]) + ((unsigned int)(((unsigned int)blur_x_0[(blur_y_s0_x + (blur_y_s0_y * 6472))]) + ((unsigned int)blur_x_0[((blur_y_s0_x + (blur_y_s0_y * 6472)) + 6472)])))) / (unsigned int)3));
    }
  }
}