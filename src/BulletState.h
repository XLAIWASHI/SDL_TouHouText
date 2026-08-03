#ifndef BULLET_STATE_H
#define BULLET_STATE_H

// 螺旋
struct SpiralState
{
    float timer = 0.0f; // 发射计时
    float angle = 0.0f; // 当前角度
};

struct RotateFanState
{
    float timer = 0.0f; // 发射计时
    float centerAngle = 0.0f; // 中心角度
};

struct SweepFanState
{
    float timer = 0.0f; // 发射计时
    float currentAngle = 0.0f; // 当前中心角度
    float sweepDir = 1.0f; // 扫射方向 1向右 -1向左
};

#endif