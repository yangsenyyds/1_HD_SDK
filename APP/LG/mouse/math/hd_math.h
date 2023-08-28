#ifndef HD_MATH
#define HD_MATH
#include <math.h>

typedef struct {
    float pitch;
    float yaw;
    float roll;
}euler_angles_t;

extern float _sqrt(float x);
extern float inv_sqrt(float number);
extern void EulerAngle_Calculate(euler_angles_t *dev, float ax, float ay, float az, float gx, float gy, float gz);
extern void EulerAngle_Calculate_ReStart(void);

#endif
