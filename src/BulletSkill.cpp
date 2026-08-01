#include "BulletSkill.h"
#include "BulletManager.h"


void BulletSkill::Spiral(SpiralState &state, float deltaTime, BulletManager &manager, SDL_FPoint position, float interval, float angleStep, BulletType type)
{
    state.timer += deltaTime;
    if(state.timer >= interval)
    {
        bulletPattern->shootDirection(manager, position, state.angle, type);
        state.timer = 0;
        state.angle += angleStep;
    }
}

void BulletSkill::RotateFan(RotateFanState &state, float deltaTime, BulletManager &manager, SDL_FPoint position, float interval, float angleStep, float spreadAngle, int cnt, BulletType type)
{
    state.timer += deltaTime;
    if(state.timer >= interval)
    {
        bulletPattern->shootFan(manager, position, state.centerAngle, spreadAngle, cnt, type);
        state.timer = 0;
        state.centerAngle += angleStep;
    }
}

void BulletSkill::Aimed(BulletManager &manager, SDL_FPoint from, SDL_FPoint to, BulletType type)
{
    bulletPattern->shootAimed(manager, from, to, type);
}

void BulletSkill::Circle(BulletManager &manager, SDL_FPoint position, int cnt, BulletType type)
{
    bulletPattern->shootCircle(manager, position, cnt, type);
}
