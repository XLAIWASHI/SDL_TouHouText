#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <SDL.h>
#include <list>

class EnemyBullet;

class BulletManager
{
public:
    BulletManager(int margin, int play_w, int play_h);
    void addBullet(EnemyBullet* bullet);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void clear();
    bool isEmpty() { return bullets.empty(); }
    std::list<EnemyBullet*>* getBullets() { return &bullets; }
private:
    std::list<EnemyBullet*> bullets;
    int margin = 0;
    int play_w = 0;
    int play_h = 0;
};

#endif