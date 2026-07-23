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

void Boss1Fight::update(float deltaTime, SDL_FPoint bossPos, BulletManager &manager, SDL_FPoint playerPos)
{
    timer += deltaTime;
    if(timer > 1.0f)
    {
        // bulletPattern->shootCircle(manager, bossPos, 50, BulletType::granBlue);
        bulletPattern->shootAimed(manager, bossPos, playerPos, BulletType::granBlue);
        timer = 0;
    }
    // if(timer > 2.0f)
    // {
    //     bulletPattern->shootFan(manager, bossPos, -90, 300, 100, BulletType::granRed);
    //     bulletPattern->shootSpread(manager, bossPos, 10, 500, BulletType::granRed);
    //     timer = 0;
    // }
}
