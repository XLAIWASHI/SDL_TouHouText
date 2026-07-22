#include "BulletPattern.h"
#include "BulletManager.h"
#include "EnemyBullet.h"

void BulletPattern::shootCircle(BulletManager &manager, SDL_FPoint position, int cnt, BulletType type)
{
    float angleStep = 360.0f / cnt;
    for(int i = 0; i < cnt; i++)
    {
        float angle = i * angleStep;
        float rad = angle * M_PI / 180.0f;
        SDL_FPoint dir;
        dir.x = cos(rad);
        dir.y = sin(rad);
        createBullet(manager, position, dir, type);
    }
}

void BulletPattern::createBullet(BulletManager &manager, SDL_FPoint position, SDL_FPoint direction, BulletType type)
{
    EnemyBullet* bullet = new EnemyBullet();
    bullet->direction = direction;
    bullet->texture = bulletTextureManager["EnemyBullet1"];
    bullet->type = type;
    SDL_QueryTexture(bullet->texture, nullptr, nullptr, &bullet->width, &bullet->height);
    bullet->position.x = position.x - bullet->width / 2;
    bullet->position.y = position.y - bullet->height / 2;
    manager.addBullet(bullet);
}
