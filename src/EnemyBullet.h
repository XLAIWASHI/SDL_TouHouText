#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "Object.h"
#include <SDL.h>

class EnemyBullet
{
public:
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint direction = {0.0f, 0.0f};
    int width = 0;
    int height = 0;
    float speed = 200;
    int damage = 1;
    float timer = 0.0f;
    float lifeTimer = 0.0f;//已经存活的时间
    float lifeTime = 6.0f;
    BulletType type = BulletType::granBlue; 
    EnemyType enemyType = EnemyType::enemyBase1;
};


#endif