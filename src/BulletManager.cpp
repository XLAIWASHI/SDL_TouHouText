#include "BulletManager.h"
#include "EnemyBullet.h"

void BulletManager::addBullet(EnemyBullet *bullet)
{
    bullets.push_back(bullet);
}

void BulletManager::update(float deltaTime)
{
    for(auto& bullet : bullets)
    {
        bullet->position.x += bullet->speed * deltaTime * bullet->direction.x;
        bullet->position.y += bullet->speed * deltaTime * bullet->direction.y;
    }
}

void BulletManager::render(SDL_Renderer *renderer)
{
    for(auto& bullet : bullets)
    {
        SDL_Rect rect = {
            static_cast<int>(bullet->position.x),
            static_cast<int>(bullet->position.y),
            bullet->width,
            bullet->height
        };
        SDL_RenderCopy(renderer, bullet->texture, nullptr, &rect);
    }
}

void BulletManager::clear()
{
}
