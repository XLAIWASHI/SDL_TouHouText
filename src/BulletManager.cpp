#include "BulletManager.h"
#include "EnemyBullet.h"

BulletManager::BulletManager(int margin, int play_w, int play_h)
{
    this->margin = margin;
    this->play_w = play_w;
    this->play_h = play_h;
}

void BulletManager::addBullet(EnemyBullet *bullet)
{
    bullets.push_back(bullet);
}

void BulletManager::update(float deltaTime)
{
    for(auto it = bullets.begin(); it != bullets.end(); )
    {
        EnemyBullet* bullet = *it;
        bullet->position.x += bullet->speed * deltaTime * bullet->direction.x;
        bullet->position.y += bullet->speed * deltaTime * bullet->direction.y;

        if(bullet->position.y > margin + play_h ||
           bullet->position.y < margin ||
           bullet->position.x < margin ||
           bullet->position.x > margin + play_w)
        {
            delete bullet;
            it = bullets.erase(it);
        }
        else
        {
            ++it;
        }
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
    for(auto b : bullets) delete b;
    bullets.clear();
}
