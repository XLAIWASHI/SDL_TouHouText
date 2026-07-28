#ifndef BOSS_FIGHT_H
#define BOSS_FIGHT_H

#include <SDL.h>
class BulletManager;

class BossFight
{
public:
    virtual ~BossFight() = default;
    virtual void update(float deltaTime, BulletManager &manager, SDL_FPoint playerPos, SDL_FPoint bossPos) = 0;

};

#endif