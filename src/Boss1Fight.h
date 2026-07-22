#ifndef BOSS1_FIGHT_H
#define BOSS1_FIGHT_H

#include <SDL.h>
#include <string>
#include <unordered_map>

class BulletPattern;
class BulletManager;

class Boss1Fight
{
public:
    Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager);
    ~Boss1Fight();
    void update(float deltaTime, SDL_FPoint bossPos, BulletManager& manager);

private:
    BulletPattern* bulletPattern;
    float timer = 0.0f;


};

#endif