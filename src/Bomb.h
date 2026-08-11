#ifndef BOMB_H
#define BOMB_H

#include <SDL.h>

class Bomb
{
public:
    Bomb(SDL_FPoint position);
    ~Bomb() = default;

    float timer = 0.0f;
    float scale = 1.0f;

    SDL_FPoint position = {0.0f, 0.0f};
    int width = 256;
    int height = 256;
    int baseWidth = 256;
    int baseHeight = 256;
    float lifeTime = 0.7f;
    float scaleSpeed = 2.0f;
    float maxScale = 6.0f;
    bool bombHitBoss = false;
    
};

#endif