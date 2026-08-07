#include "BossFightController.h"
#include "Boss.h"
#include "BossFight.h"
#include "Boss1Fight.h"
#include "BulletManager.h"


void BossFightController::createFight(BossType type, Boss *boss)
{
    if(boss == nullptr)
    {
        return;
    }
    if(currentBossFight != nullptr)
    {
        delete currentBossFight;
        currentBossFight = nullptr;
    }
    this->boss = boss;
    this->playerPos = playerPos;
    switch(type)
    {
        case BossType::boss1:
            currentBossFight = new Boss1Fight(textureManager, boss);
            break;
    }
}

void BossFightController::update(float deltaTime, BulletManager &manager, SDL_FPoint playerPos)
{
    if(currentBossFight == nullptr || boss == nullptr)
    {
        return;
    }
    this->playerPos = playerPos;
    currentBossFight->update(deltaTime, manager, this->playerPos, boss->getBossPosition());
}

bool BossFightController::hasBossStage() const
{
    return currentBossFight != nullptr && currentBossFight->hasBossStage();
}

int BossFightController::getStageCurrentHP() const
{
    return currentBossFight ? currentBossFight->getStageCurrentHP() : 0;
}

int BossFightController::getStageMaxHP() const
{
    return currentBossFight ? currentBossFight->getStageMaxHP() : 0;
}

float BossFightController::getStageTimer() const
{
    return currentBossFight ? currentBossFight->getStageTimer() : 0.0f;
}
