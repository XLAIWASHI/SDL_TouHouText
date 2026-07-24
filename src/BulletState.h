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

#endif