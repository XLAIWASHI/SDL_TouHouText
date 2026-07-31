#include "Boss1Fight.h"
#include "BulletManager.h"
#include "BulletSkill.h"
#include "Boss.h"

Boss1Fight::Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager, Boss* boss)
{
    bulletSkill = new BulletSkill(textureManager);
    rotateFanState.centerAngle = 90.0f;
    this->boss = boss;
    changeStage(Boss1Stage::Enter);
}

Boss1Fight::~Boss1Fight()
{
    delete bulletSkill;
}

void Boss1Fight::update(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos)
{
    stageTimer += deltaTime;

    switch(stage)
    {
    case Boss1Stage::Enter:
        updateEnter(deltaTime);
        break;
    case Boss1Stage::Pattern1:
        updatePattern1(deltaTime, manager, playerPos, bossPos);
        break;
    case Boss1Stage::Pattern2:
        break;
    case Boss1Stage::Pattern3:
        break;
    case Boss1Stage::Dead:
        break;
    }
}

void Boss1Fight::changeStage(Boss1Stage newStage)
{
    //exit旧阶段
    switch(stage)
    {
    case Boss1Stage::None:
        exitNone();
        break;
    case Boss1Stage::Enter:
        exitEnter();
        break;
    case Boss1Stage::Pattern1:
        exitPattern1();
        break;
    
    default:
        break;    
    }

    stage = newStage;
    stageTimer = 0;

    //enter新阶段
    switch(stage)
    {
    case Boss1Stage::Enter:
        enter();
        break;
    case Boss1Stage::Pattern1:
        enterPattern1();
        break;

    default:
        break;
    }
}

void Boss1Fight::exitNone()
{
}

void Boss1Fight::enter()
{
    boss->setTargetPosition(300.0f, 300.0f);
}

void Boss1Fight::updateEnter(float deltaTime)
{
    boss->startEnter();
    if(stageTimer > 2.0f)
    {
        changeStage(Boss1Stage::Pattern1);
    }
}

void Boss1Fight::exitEnter()
{
    
}

void Boss1Fight::enterPattern1()
{
    spiralState.timer = 0.0f;
    spiralState.angle = 0.0f;
}

void Boss1Fight::updatePattern1(float deltaTime, BulletManager &manager, SDL_FPoint playerPos, SDL_FPoint bossPos)
{
    bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.01f, 10, BulletType::granBlue);
    bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 1.0f, 90, 60, 10, BulletType::granBlue);
    if(stageTimer >= 0 && stageTimer <= 1)
        boss->randomMove();
    if(stageTimer > 15.0f)
    {
        
        changeStage(Boss1Stage::Pattern2);
    }
}

void Boss1Fight::exitPattern1()
{
    
}
