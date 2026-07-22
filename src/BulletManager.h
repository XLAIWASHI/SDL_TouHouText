#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <SDL.h>
#include <list>

class EnemyBullet;

class BulletManager
{
public:
    void addBullet(EnemyBullet* bullet);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void clear();
private:
    std::list<EnemyBullet*> bullets;
};

#endif