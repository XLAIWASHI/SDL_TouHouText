#ifndef BULLET_PATTERN_H
#define BULLET_PATTERN_H

#include "Object.h"
#include <SDL.h>
#include <list>
#include <string>

class EnemyBullet;
class BulletManager;

class BulletPattern
{
public:
    BulletPattern(std::unordered_map<std::string, SDL_Texture*>& textureManager) : bulletTextureManager(textureManager)
    {
    }
    void shootCircle(BulletManager& manager, SDL_FPoint position, int cnt, BulletType type);



private:
    void createBullet(BulletManager& manager, SDL_FPoint position, SDL_FPoint direction, BulletType type);
    std::unordered_map<std::string, SDL_Texture*>& bulletTextureManager;
};


#endif