#include "Boss1Fight.h"
#include "BulletManager.h"
#include "BulletPattern.h"

Boss1Fight::Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager)
{
    bulletPattern = new BulletPattern(textureManager);
    
}

Boss1Fight::~Boss1Fight()
{
    delete bulletPattern;
}

void Boss1Fight::update(float deltaTime, SDL_FPoint bossPos, BulletManager &manager)
{
    timer += deltaTime;
    if(timer > 1.0f)
    {
        bulletPattern->shootCircle(manager, bossPos, 10, BulletType::granBlue);
        timer = 0;
    }
}
