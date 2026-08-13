#include "Boss1Fight.h"
#include "BulletManager.h"
#include "BulletSkill.h"
#include "Boss.h"
#include "EffectManager.h"

Boss1Fight::Boss1Fight(std::unordered_map<std::string, SDL_Texture*>& textureManager, EffectManager& effectManager, Boss* boss)
: effectManager(effectManager)
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
        updatePattern2(deltaTime, manager, playerPos, bossPos);
        break;
    case Boss1Stage::Pattern3:
        updatePattern3(deltaTime, manager, playerPos, bossPos);
        break;
    case Boss1Stage::Dead:
        break;
    }
}

void Boss1Fight::changeStage(Boss1Stage newStage)
{
    switch(stage)
    {
    case Boss1Stage::None:    exitNone();    break;
    case Boss1Stage::Enter:   exitEnter();   break;
    case Boss1Stage::Pattern1: exitPattern1(); break;
    case Boss1Stage::Pattern2: exitPattern2(); break;
    case Boss1Stage::Pattern3: exitPattern3(); break;
    case Boss1Stage::Dead:    exitDead();    break;
    default: break;
    }

    stage = newStage;
    stageTimer = 0;

    switch(stage)
    {
    case Boss1Stage::Enter:   enter();   break;
    case Boss1Stage::Pattern1: enterPattern1(); break;
    case Boss1Stage::Pattern2: enterPattern2(); break;
    case Boss1Stage::Pattern3: enterPattern3(); break;
    default: break;
    }
}

void Boss1Fight::exitNone() {}
void Boss1Fight::exitEnter() {}

void Boss1Fight::enter()
{
    boss->setTargetPosition(300.0f, 200.0f);
}

void Boss1Fight::updateEnter(float deltaTime)
{
    boss->startEnter();
    if(stageTimer > 2.0f)
        changeStage(Boss1Stage::Pattern1);
}

//==================== Pattern1 ====================

void Boss1Fight::enterPattern1()
{
    spiralState.timer = 0.0f;
    spiralState.angle = 0.0f;
    rotateFanState.centerAngle = 90.0f;
    rotateFanState.timer = 0.0f;
    aimedTimer = 0.0f;
    circleTimer = 0.0f;
    p1SubPhase = Pattern1SubPhase::AttackWarmup;
    p1SubTimer = 0.0f;
    p1HealthAtStart = boss->getHealth();
    boss->startAttackAnimation();
}

void Boss1Fight::updatePattern1(float deltaTime, BulletManager &manager, SDL_FPoint playerPos, SDL_FPoint bossPos)
{
    int dmg = p1HealthAtStart - boss->getHealth();
    if(dmg >= 1500)
    {
        changeStage(Boss1Stage::Pattern2);
        return;
    }

    p1SubTimer += deltaTime;

    switch(p1SubPhase)
    {
    case Pattern1SubPhase::AttackWarmup:
        boss->startAttackAnimation();
        if(p1SubTimer >= 1.0f) { p1SubPhase = Pattern1SubPhase::SkillA; p1SubTimer = 0.0f; }
        return;

    case Pattern1SubPhase::SkillA:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.01f, 10, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 1.0f, 90, 60, 10, BulletType::granBlue);
        if(p1SubTimer >= 3.0f) { p1SubPhase = Pattern1SubPhase::Move1; p1SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern1SubPhase::Move1:
        if(p1SubTimer >= 2.0f)
        {
            p1SubPhase = Pattern1SubPhase::SkillB; p1SubTimer = 0.0f;
            spiralState.angle = 0.0f; spiralState.timer = 0.0f; aimedTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern1SubPhase::SkillB:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.012f, 12, BulletType::granBlue);
        aimedTimer += deltaTime;
        if(aimedTimer >= 0.6f) { aimedTimer = 0.0f; bulletSkill->Aimed(manager, bossPos, playerPos, BulletType::granRed); }
        if(p1SubTimer >= 3.0f) { p1SubPhase = Pattern1SubPhase::Move2; p1SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern1SubPhase::Move2:
        if(p1SubTimer >= 2.0f)
        {
            p1SubPhase = Pattern1SubPhase::SkillC; p1SubTimer = 0.0f;
            rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern1SubPhase::SkillC:
        boss->startAttackAnimation();
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.8f, 90, 70, 12, BulletType::granBlue);
        if(p1SubTimer >= 3.0f) { p1SubPhase = Pattern1SubPhase::SkillF; p1SubTimer = 0.0f; sweepFanState.currentAngle = 45.0f; sweepFanState.timer = 0.0f; sweepFanState.sweepDir = 1.0f; boss->startAttackAnimation(); }
        break;

    case Pattern1SubPhase::SkillF:
        boss->startAttackAnimation();
        bulletSkill->Sweep(sweepFanState, deltaTime, manager, bossPos, 0.01f, 10, 45.0f, 135.0f, BulletType::granBlue);
        if(p1SubTimer >= 5.0f) { p1SubPhase = Pattern1SubPhase::Move3; p1SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern1SubPhase::Move3:
        if(p1SubTimer >= 2.0f)
        {
            p1SubPhase = Pattern1SubPhase::SkillD; p1SubTimer = 0.0f;
            spiralState.angle = 0.0f; spiralState.timer = 0.0f; circleTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern1SubPhase::SkillD:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.015f, 15, BulletType::granBlue);
        circleTimer += deltaTime;
        if(circleTimer >= 1.0f) { circleTimer = 0.0f; bulletSkill->Circle(manager, bossPos, 12, BulletType::granRed); }
        if(p1SubTimer >= 3.0f) { p1SubPhase = Pattern1SubPhase::Move4; p1SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern1SubPhase::Move4:
        if(p1SubTimer >= 2.0f)
        {
            p1SubPhase = Pattern1SubPhase::SkillE; p1SubTimer = 0.0f;
            sweepFanState.currentAngle = 30.0f; sweepFanState.timer = 0.0f; sweepFanState.sweepDir = 1.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern1SubPhase::SkillE:
        boss->startAttackAnimation();
        bulletSkill->SweepFan(sweepFanState, deltaTime, manager, bossPos, 0.2f, 30, 8, 30.0f, 150.0f, 60.0f, BulletType::granBlue);
        if(p1SubTimer >= 4.0f) { p1SubPhase = Pattern1SubPhase::Done; }
        break;

    case Pattern1SubPhase::Done:
        changeStage(Boss1Stage::Pattern2);
        break;
    }
}

void Boss1Fight::exitPattern1() {}

//==================== Pattern2 ====================

void Boss1Fight::enterPattern2()
{
    spiralState.timer = 0.0f; spiralState.angle = 0.0f;
    rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
    aimedTimer = 0.0f; circleTimer = 0.0f;
    p2SubPhase = Pattern2SubPhase::AttackWarmup;
    p2SubTimer = 0.0f;
    p2HealthAtStart = boss->getHealth();
    boss->startAttackAnimation();
}

void Boss1Fight::updatePattern2(float deltaTime, BulletManager &manager, SDL_FPoint playerPos, SDL_FPoint bossPos)
{
    int dmg = p2HealthAtStart - boss->getHealth();
    if(dmg >= 1500)
    {
        changeStage(Boss1Stage::Pattern3);
        return;
    }

    p2SubTimer += deltaTime;

    switch(p2SubPhase)
    {
    case Pattern2SubPhase::AttackWarmup:
        boss->startAttackAnimation();
        if(p2SubTimer >= 1.0f) { p2SubPhase = Pattern2SubPhase::SkillA; p2SubTimer = 0.0f; }
        return;

    case Pattern2SubPhase::SkillA:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.008f, 15, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.8f, 90, 80, 12, BulletType::granBlue);
        if(p2SubTimer >= 3.5f) { p2SubPhase = Pattern2SubPhase::Move1; p2SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern2SubPhase::Move1:
        if(p2SubTimer >= 2.0f)
        {
            p2SubPhase = Pattern2SubPhase::SkillB; p2SubTimer = 0.0f;
            spiralState.angle = 0.0f; spiralState.timer = 0.0f; aimedTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern2SubPhase::SkillB:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.01f, 20, BulletType::granBlue);
        aimedTimer += deltaTime;
        if(aimedTimer >= 0.5f) { aimedTimer = 0.0f; bulletSkill->Aimed(manager, bossPos, playerPos, BulletType::granRed); }
        if(p2SubTimer >= 3.5f) { p2SubPhase = Pattern2SubPhase::SkillF; p2SubTimer = 0.0f; sweepFanState.currentAngle = 0.0f; sweepFanState.timer = 0.0f; sweepFanState.sweepDir = 1.0f; boss->startAttackAnimation(); }
        break;

    case Pattern2SubPhase::SkillF:
        boss->startAttackAnimation();
        bulletSkill->Sweep(sweepFanState, deltaTime, manager, bossPos, 0.008f, 15, 0.0f, 180.0f, BulletType::granBlue);
        if(p2SubTimer >= 8.0f) { p2SubPhase = Pattern2SubPhase::Move2; p2SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern2SubPhase::Move2:
        if(p2SubTimer >= 2.0f)
        {
            p2SubPhase = Pattern2SubPhase::SkillC; p2SubTimer = 0.0f;
            rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
            rotateFanState2.centerAngle = 270.0f; rotateFanState2.timer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern2SubPhase::SkillC:
        boss->startAttackAnimation();
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.6f, 90, 90, 15, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState2, deltaTime, manager, bossPos, 0.6f, 90, 90, 15, BulletType::granBlue);
        if(p2SubTimer >= 3.5f) { p2SubPhase = Pattern2SubPhase::Move3; p2SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern2SubPhase::Move3:
        if(p2SubTimer >= 2.0f)
        {
            p2SubPhase = Pattern2SubPhase::SkillD; p2SubTimer = 0.0f;
            spiralState.angle = 0.0f; spiralState.timer = 0.0f;
            spiralState2.angle = 120.0f; spiralState2.timer = 0.0f;
            spiralState3.angle = 240.0f; spiralState3.timer = 0.0f;
            aimedTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern2SubPhase::SkillD:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.01f, 12, BulletType::granBlue);
        bulletSkill->Spiral(spiralState2, deltaTime, manager, bossPos, 0.01f, 12, BulletType::granBlue);
        bulletSkill->Spiral(spiralState3, deltaTime, manager, bossPos, 0.01f, 12, BulletType::granBlue);
        aimedTimer += deltaTime;
        if(aimedTimer >= 0.6f) { aimedTimer = 0.0f; bulletSkill->Aimed(manager, bossPos, playerPos, BulletType::granRed); }
        if(p2SubTimer >= 3.5f) { p2SubPhase = Pattern2SubPhase::Move4; p2SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern2SubPhase::Move4:
        if(p2SubTimer >= 2.0f)
        {
            p2SubPhase = Pattern2SubPhase::SkillE; p2SubTimer = 0.0f;
            sweepFanState.currentAngle = 45.0f; sweepFanState.timer = 0.0f; sweepFanState.sweepDir = 1.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern2SubPhase::SkillE:
        boss->startAttackAnimation();
        bulletSkill->SweepFan(sweepFanState, deltaTime, manager, bossPos, 0.15f, 40, 12, 45.0f, 135.0f, 80.0f, BulletType::granBlue);
        if(p2SubTimer >= 4.5f) { p2SubPhase = Pattern2SubPhase::Done; }
        break;

    case Pattern2SubPhase::Done:
        changeStage(Boss1Stage::Pattern3);
        break;
    }
}

void Boss1Fight::exitPattern2() {}

//==================== Pattern3 ====================

void Boss1Fight::enterPattern3()
{
    spiralState.timer = 0.0f; spiralState.angle = 0.0f;
    spiralState2.timer = 0.0f; spiralState2.angle = 120.0f;
    spiralState3.timer = 0.0f; spiralState3.angle = 240.0f;
    rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
    aimedTimer = 0.0f; circleTimer = 0.0f;
    p3SubPhase = Pattern3SubPhase::AttackWarmup;
    p3SubTimer = 0.0f;
    p3HealthAtStart = boss->getHealth();
    boss->startAttackAnimation();
}

void Boss1Fight::updatePattern3(float deltaTime, BulletManager &manager, SDL_FPoint playerPos, SDL_FPoint bossPos)
{
    int dmg = p3HealthAtStart - boss->getHealth();
    if(dmg >= 1500 || boss->getHealth() <= 0)
    {
        game.playSound(game.getSounds()["enep01"], -1);
        effectManager.addEffect(bossPos, EffectType::enemyDead);
        boss->setDead();
        changeStage(Boss1Stage::Dead);
        return;
    }

    p3SubTimer += deltaTime;

    switch(p3SubPhase)
    {
    case Pattern3SubPhase::AttackWarmup:
        boss->startAttackAnimation();
        if(p3SubTimer >= 1.0f) { p3SubPhase = Pattern3SubPhase::SkillA; p3SubTimer = 0.0f; }
        return;

    case Pattern3SubPhase::SkillA:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.01f, 12, BulletType::granBlue);
        bulletSkill->Spiral(spiralState2, deltaTime, manager, bossPos, 0.01f, 12, BulletType::granBlue);
        bulletSkill->Spiral(spiralState3, deltaTime, manager, bossPos, 0.01f, 12, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.8f, 90, 60, 10, BulletType::granBlue);
        if(p3SubTimer >= 4.0f) { p3SubPhase = Pattern3SubPhase::Move1; p3SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern3SubPhase::Move1:
        if(p3SubTimer >= 1.5f)
        {
            p3SubPhase = Pattern3SubPhase::SkillB; p3SubTimer = 0.0f;
            rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
            circleTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern3SubPhase::SkillB:
        boss->startAttackAnimation();
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.5f, 90, 120, 20, BulletType::granBlue);
        circleTimer += deltaTime;
        if(circleTimer >= 1.0f) { circleTimer = 0.0f; bulletSkill->Circle(manager, bossPos, 16, BulletType::granRed); }
        if(p3SubTimer >= 4.5f) { p3SubPhase = Pattern3SubPhase::Move2; p3SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern3SubPhase::Move2:
        if(p3SubTimer >= 1.5f)
        {
            p3SubPhase = Pattern3SubPhase::SkillC; p3SubTimer = 0.0f;
            spiralState.timer = 0.0f; spiralState.angle = 0.0f;
            aimedTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern3SubPhase::SkillC:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.005f, 30, BulletType::granBlue);
        aimedTimer += deltaTime;
        if(aimedTimer >= 0.3f) { aimedTimer = 0.0f; bulletSkill->Aimed(manager, bossPos, playerPos, BulletType::granRed); }
        if(p3SubTimer >= 4.5f) { p3SubPhase = Pattern3SubPhase::Move3; p3SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern3SubPhase::Move3:
        if(p3SubTimer >= 1.5f)
        {
            p3SubPhase = Pattern3SubPhase::SkillD; p3SubTimer = 0.0f;
            rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
            rotateFanState2.centerAngle = 270.0f; rotateFanState2.timer = 0.0f;
            circleTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern3SubPhase::SkillD:
        boss->startAttackAnimation();
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.5f, 90, 90, 18, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState2, deltaTime, manager, bossPos, 0.5f, 90, 90, 18, BulletType::granBlue);
        circleTimer += deltaTime;
        if(circleTimer >= 1.5f) { circleTimer = 0.0f; bulletSkill->Circle(manager, bossPos, 16, BulletType::granRed); }
        if(p3SubTimer >= 4.5f) { p3SubPhase = Pattern3SubPhase::SkillG; p3SubTimer = 0.0f; sweepFanState.currentAngle = 0.0f; sweepFanState.timer = 0.0f; sweepFanState.sweepDir = 1.0f; boss->startAttackAnimation(); }
        break;

    case Pattern3SubPhase::SkillG:
        boss->startAttackAnimation();
        bulletSkill->Sweep(sweepFanState, deltaTime, manager, bossPos, 0.006f, 20, 0.0f, 360.0f, BulletType::granBlue);
        if(p3SubTimer >= 15.0f) { p3SubPhase = Pattern3SubPhase::Move4; p3SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern3SubPhase::Move4:
        if(p3SubTimer >= 1.0f)
        {
            p3SubPhase = Pattern3SubPhase::SkillE; p3SubTimer = 0.0f;
            spiralState.timer = 0.0f; spiralState.angle = 0.0f;
            spiralState2.timer = 0.0f; spiralState2.angle = 120.0f;
            spiralState3.timer = 0.0f; spiralState3.angle = 240.0f;
            rotateFanState.centerAngle = 90.0f; rotateFanState.timer = 0.0f;
            rotateFanState2.centerAngle = 270.0f; rotateFanState2.timer = 0.0f;
            aimedTimer = 0.0f; circleTimer = 0.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern3SubPhase::SkillE:
        boss->startAttackAnimation();
        bulletSkill->Spiral(spiralState, deltaTime, manager, bossPos, 0.008f, 15, BulletType::granBlue);
        bulletSkill->Spiral(spiralState2, deltaTime, manager, bossPos, 0.008f, 15, BulletType::granBlue);
        bulletSkill->Spiral(spiralState3, deltaTime, manager, bossPos, 0.008f, 15, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState, deltaTime, manager, bossPos, 0.5f, 90, 90, 16, BulletType::granBlue);
        bulletSkill->RotateFan(rotateFanState2, deltaTime, manager, bossPos, 0.5f, 90, 90, 16, BulletType::granBlue);
        aimedTimer += deltaTime;
        if(aimedTimer >= 0.4f) { aimedTimer = 0.0f; bulletSkill->Aimed(manager, bossPos, playerPos, BulletType::granRed); }
        circleTimer += deltaTime;
        if(circleTimer >= 1.5f) { circleTimer = 0.0f; bulletSkill->Circle(manager, bossPos, 20, BulletType::granRed); }
        if(p3SubTimer >= 6.0f) { p3SubPhase = Pattern3SubPhase::Move5; p3SubTimer = 0.0f; boss->randomMove(); }
        break;

    case Pattern3SubPhase::Move5:
        if(p3SubTimer >= 1.0f)
        {
            p3SubPhase = Pattern3SubPhase::SkillF; p3SubTimer = 0.0f;
            sweepFanState.currentAngle = 0.0f; sweepFanState.timer = 0.0f; sweepFanState.sweepDir = 1.0f;
            boss->startAttackAnimation();
        }
        break;

    case Pattern3SubPhase::SkillF:
        boss->startAttackAnimation();
        bulletSkill->SweepFan(sweepFanState, deltaTime, manager, bossPos, 0.1f, 50, 16, 0.0f, 720.0f, 120.0f, BulletType::granBlue);
        if(p3SubTimer >= 5.0f) { p3SubPhase = Pattern3SubPhase::Done; }
        break;

    case Pattern3SubPhase::Done:
        game.playSound(game.getSounds()["enep01"], -1);
        effectManager.addEffect(bossPos, EffectType::enemyDead);
        boss->setDead();
        changeStage(Boss1Stage::Dead);
        break;
    }
}

void Boss1Fight::exitPattern3() {}

void Boss1Fight::exitDead() {}

int Boss1Fight::getStageCurrentHP() const
{
    switch(stage)
    {
    case Boss1Stage::Pattern1:
        return STAGE_MAX_HP - (p1HealthAtStart - boss->getHealth());
    case Boss1Stage::Pattern2:
        return STAGE_MAX_HP - (p2HealthAtStart - boss->getHealth());
    case Boss1Stage::Pattern3:
        return STAGE_MAX_HP - (p3HealthAtStart - boss->getHealth());
    default:
        return 0;
    }
}
