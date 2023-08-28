#include "hd_math.h"
#include "app_config.h"

#define Kp      (10.0f)               
#define Ki      (0.008f)            
#define halfT   (0.004f)

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static float exInt, eyInt, ezInt;

float _sqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5f - xhalf * x * x);

    return 1.0 / x;
}

float inv_sqrt(float number)
{
    volatile long i;
    volatile float x, y;
    volatile const float f = 1.5F;

    x = number * 0.5F;
    y = number;
    i = * (( long * ) &y);
    i = 0x5f375a86 - ( i >> 1 );
    y = * (( float * ) &i);
    y = y * ( f - ( x * y * y ) );
    return y;
}

void EulerAngle_Calculate(euler_angles_t *dev, float ax, float ay, float az, float gx, float gy, float gz)
{
    float recipNorm;
    float vx, vy, vz;
    float ex, ey, ez;
    float q0_temp, q1_temp, q2_temp, q3_temp;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;

    float q1q1 = q1*q1;
    float q1q3 = q1*q3;

    float q2q2 = q2*q2;
    float q2q3 = q2*q3;

    float q3q3 = q3*q3;

    if(az == 0) {
        return;
    }
    
    /* 对加速度数据进行归一化处理 */
    recipNorm = inv_sqrt(ax*ax + ay*ay + az*az);
    ax = ax * recipNorm;
    ay = ay * recipNorm;
    az = az * recipNorm;
    /* DCM矩阵旋转 */
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;
    /* 在机体坐标系下做向量叉积得到补偿数据 */
    ex = ay*vz - az*vy ;
    ey = az*vx - ax*vz ;
    ez = ax*vy - ay*vx ;
    /* 对误差进行PI计算，补偿角速度 */
    exInt = exInt + ex*Ki;
    eyInt = eyInt + ey*Ki;
    ezInt = ezInt + ez*Ki;

    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;
    /* 按照四元素微分公式进行四元素更新 */
    q0_temp = q0;
    q1_temp = q1;
    q2_temp = q2;
    q3_temp = q3;

    q0 = q0_temp + (-q1_temp*gx - q2_temp*gy - q3_temp*gz)*halfT;
    q1 = q1_temp + (q0_temp*gx + q2_temp*gz - q3_temp*gy)*halfT;
    q2 = q2_temp + (q0_temp*gy - q1_temp*gz + q3_temp*gx)*halfT;
    q3 = q3_temp + (q0_temp*gz + q1_temp*gy - q2_temp*gx)*halfT;

    recipNorm = inv_sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 * recipNorm;
    q1 = q1 * recipNorm;
    q2 = q2 * recipNorm;
    q3 = q3 * recipNorm;
    
    dev->pitch = asinf(2*q1*q3 - 2*q0*q2)*57.3f;
    dev->yaw = -atan2f(2*q1*q2 + 2*q0*q3, -2*q2*q2 -2*q3*q3 + 1)*57.3f;
    // dev->roll = atan2f(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)*57.3f;
}

void EulerAngle_Calculate_ReStart(void)
{
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;

    exInt = 0.0f;
    eyInt = 0.0f;
    ezInt = 0.0f;
}