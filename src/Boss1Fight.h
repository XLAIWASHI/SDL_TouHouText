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

enum class Pattern1SubPhase
{
    AttackWarmup,
    SkillA,
    Move1,
    SkillB,
    Move2,
    SkillC,
    Move3,
    SkillD,
    Done
};

enum class Pattern2SubPhase
{
    AttackWarmup,
    SkillA,
    Move1,
    SkillB,
    Move2,
    SkillC,
    Move3,
    SkillD,
    Done
};

enum class Pattern3SubPhase
{
    AttackWarmup,
    SkillA,
    Move1,
    SkillB,
    Move2,
    SkillC,
    Move3,
    SkillD,
    Move4,
    SkillE,
    Done
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

    void exitNone();

    void enter();
    void updateEnter(float deltaTime);
    void exitEnter();

    void enterPattern1();
    void updatePattern1(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos);
    void exitPattern1();

    void enterPattern2();
    void updatePattern2(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos);
    void exitPattern2();

    void enterPattern3();
    void updatePattern3(float deltaTime, BulletManager& manager, SDL_FPoint playerPos, SDL_FPoint bossPos);
    void exitPattern3();

    void exitDead();

private:
    BulletSkill* bulletSkill;
    float stageTimer = 0.0f;
    SpiralState spiralState;
    SpiralState spiralState2;
    SpiralState spiralState3;
    RotateFanState rotateFanState;
    RotateFanState rotateFanState2;
    float aimedTimer = 0.0f;
    float circleTimer = 0.0f;
    Boss* boss = nullptr;
    Boss1Stage stage = Boss1Stage::None;
    int PLAYAREA_W = 0;
    int PLAYAREA_H = 0;
    Pattern1SubPhase p1SubPhase = Pattern1SubPhase::AttackWarmup;
    float p1SubTimer = 0.0f;
    int p1HealthAtStart = 0;
    Pattern2SubPhase p2SubPhase = Pattern2SubPhase::AttackWarmup;
    float p2SubTimer = 0.0f;
    int p2HealthAtStart = 0;
    Pattern3SubPhase p3SubPhase = Pattern3SubPhase::AttackWarmup;
    float p3SubTimer = 0.0f;
    int p3HealthAtStart = 0;
};

#endif
