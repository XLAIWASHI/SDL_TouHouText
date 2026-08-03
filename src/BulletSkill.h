#ifndef BULLET_SKILL_H
#define BULLET_SKILL_H

#include "BulletState.h"
#include "BulletPattern.h"
#include "Object.h"
#include <sdl.h>
#include <string>
#include <unordered_map>

class BulletManager;
class BulletPattern;

class BulletSkill
{
public:
    BulletSkill(std::unordered_map<std::string, SDL_Texture*>& textureManager) : bulletTextureManager(textureManager)
    {
        bulletPattern = new BulletPattern(bulletTextureManager);
    }
    void Spiral(SpiralState& state, float deltaTime, BulletManager& manager, SDL_FPoint position,
                float interval, float angleStep, BulletType type); // interval发射间隔 螺旋
    void RotateFan(RotateFanState& state, float deltaTime, BulletManager& manager, SDL_FPoint position,
                float interval, float angleStep, float spreadAngle, int cnt, BulletType type); // 旋转扇形
    void SweepFan(SweepFanState& state, float deltaTime, BulletManager& manager, SDL_FPoint position,
                float interval, float spreadAngle, int cnt, float minAngle, float maxAngle, float sweepSpeed, BulletType type); // 扫射
    void Sweep(SweepFanState& state, float deltaTime, BulletManager& manager, SDL_FPoint position,
                float interval, float angleStep, float minAngle, float maxAngle, BulletType type); // 单发扫射
    void Aimed(BulletManager& manager, SDL_FPoint from, SDL_FPoint to, BulletType type); // 自机狙
    void Circle(BulletManager& manager, SDL_FPoint position, int cnt, BulletType type); // 全圆

private:
    BulletPattern* bulletPattern = nullptr;
    std::unordered_map<std::string, SDL_Texture*>& bulletTextureManager;
};

#endif