#ifndef BOSS1_FIGHT_H
#define BOSS1_FIGHT_H

#include "BossFight.h"
#include <SDL.h>
#include <string>
#include <unordered_map>

class BulletPattern;
class BulletManager;

class Boss1Fight : public BossFight
{
public:
    Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager);
    ~Boss1Fight();
    void update(float deltaTime, SDL_FPoint bossPos, BulletManager& manager, SDL_FPoint playerPos) override;

private:
    BulletPattern* bulletPattern;
    float timer = 0.0f;


};

#endif