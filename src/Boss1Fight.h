#ifndef BOSS1_FIGHT_H
#define BOSS1_FIGHT_H

#include "BossFight.h"
#include "BulletState.h"
#include <SDL.h>
#include <string>
#include <unordered_map>

enum class Boss1Stage
{
    None,
    Enter,
    Pattern1,
    Pattern2,
    Pattern3,
    Dead
};

class BulletSkill;
class BulletManager;
class Boss;

class Boss1Fight : public BossFight
{
public:
    Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager, Boss* boss);
    ~Boss1Fight();
    void update(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos) override;

private:
    void changeStage(Boss1Stage newStage);

    //None
    void exitNone();

    //enter
    void enter();
    void updateEnter(float deltaTime);
    void exitEnter();

    //pattern1
    void enterPattern1();
    void updatePattern1(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos);
    void exitPattern1();

private:
    BulletSkill* bulletSkill;
    float stageTimer = 0.0f;
    SpiralState spiralState;
    RotateFanState rotateFanState;
    Boss* boss = nullptr;
    Boss1Stage stage = Boss1Stage::None;
    int PLAYAREA_W = 0;
    int PLAYAREA_H = 0;
};

#endif