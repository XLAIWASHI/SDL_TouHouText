#ifndef BOSS_FIGHT_CONTROLLER_H
#define BOSS_FIGHT_CONTROLLER_H

#include "Object.h"
#include "EffectManager.h"
#include <unordered_map>
#include <string>

class Boss;
class BossFight;
class BulletManager;

class BossFightController
{
public:
    BossFightController(std::unordered_map<std::string, SDL_Texture*>& textureManager, EffectManager& effectManager) :
textureManager(textureManager), effectManager(effectManager)
{
}
    void createFight(BossType type, Boss *boss);
    void update(float deltaTime, BulletManager &manager, SDL_FPoint playerPos);

    bool hasBossStage() const;
    int getStageCurrentHP() const;
    int getStageMaxHP() const;
    float getStageTimer() const;

private:
    EffectManager& effectManager;
    SDL_FPoint playerPos = {0.0f, 0.0f};
    Boss* boss = nullptr;
    BossFight* currentBossFight = nullptr;
    std::unordered_map<std::string, SDL_Texture*>& textureManager;
};

#endif