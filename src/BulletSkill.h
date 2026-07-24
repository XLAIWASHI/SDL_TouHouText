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

private:
    BulletPattern* bulletPattern = nullptr;
    std::unordered_map<std::string, SDL_Texture*>& bulletTextureManager;
};

#endif