#include "Boss1Fight.h"
#include "BulletManager.h"
#include "BulletSkill.h"

Boss1Fight::Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager)
{
    bulletSkill = new BulletSkill(textureManager);
    rotateFanState.centerAngle = 90.0f;
}

Boss1Fight::~Boss1Fight()
{
    delete bulletSkill;
}

void Boss1Fight::update(float deltaTime, SDL_FPoint bossPos, BulletManager &manager, SDL_FPoint playerPos)
{
    fightTimer += deltaTime;
    if(fightTimer < 10.0f)
    {
        // bulletPattern->shootCircle(manager, bossPos, 50, BulletType::granBlue);
        //bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.01, 5, BulletType::granBlue);
        
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 1.0f, 90, 60, 10, BulletType::granBlue);
        // timer = 0;
    }
    // if(timer > 2.0f)
    // {
    //     bulletPattern->shootFan(manager, bossPos, -90, 300, 100, BulletType::granRed);
    //     bulletPattern->shootSpread(manager, bossPos, 10, 500, BulletType::granRed);
    //     timer = 0;
    // }
}
