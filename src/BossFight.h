#ifndef BOSS_FIGHT_H
#define BOSS_FIGHT_H

#include <SDL.h>
class BulletManager;

class BossFight
{
public:
    virtual ~BossFight() = default;
    virtual void update(float deltaTime, SDL_FPoint bossPos, BulletManager &manager, SDL_FPoint playerPos) = 0;

};

#endif