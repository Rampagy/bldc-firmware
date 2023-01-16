#ifndef ARM_ATAN2_F32_H
#define ARM_ATAN2_F32_H

#include <arm_math.h>

/**
  @brief  Arc tangent in radian of y/x using sign of x and y to determine right quadrant.
  @param[in]   y  y coordinate
  @param[in]   x  x coordinate
  @param[out]  result  Result
  @return  error status.
*/
arm_status arm_atan2_f32(float32_t y,float32_t x,float32_t *result);


#endif