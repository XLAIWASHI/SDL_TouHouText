#ifndef BOSS_FIGHT_H
#define BOSS_FIGHT_H

#include "Game.h"
#include "EffectManager.h"
#include <SDL.h>
class BulletManager;

class BossFight
{
public:
    BossFight() : game(Game::getInstance()) {}
    virtual ~BossFight() = default;
    virtual void update(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos) = 0;

    virtual bool hasBossStage() const { return false; }
    virtual int getStageCurrentHP() const { return 0; }
    virtual int getStageMaxHP() const { return 0; }
    virtual float getStageTimer() const { return 0.0f; }

protected:
    Game& game;
};

#endif