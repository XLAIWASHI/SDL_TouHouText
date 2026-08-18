#ifndef BOSS_H
#define BOSS_H

#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Pattern;

class Boss
{
public:
    Boss(int play_w, int play_h, int margin);
    ~Boss();
    bool init(SDL_Texture* texture, float x, float y);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void takeDamage(int damage); //收到伤害
    void startEnter();
    void setTargetPosition(float x, float y); //设置目标位置
    SDL_FPoint getBossPosition() { return {position.x + static_cast<int>(width * 1.5 / 2),
        position.y + static_cast<int>(height * 1.5 / 2)}; }
    BossType getBossType() { return type; }

    //getting
    int getBossWidth() { return width; }
    int getBossHeight() { return height; }
    SDL_FPoint getBossPos() { return position; }
    int getHealth() { return health; }
    bool isBossDead() { return isDead; }
    bool getStageDefeated() { return stageDefeated; }

    //setting
    void setDead() { isDead = true; }
    void setStageDefeated(bool f) { stageDefeated = f; }
    
    //随机移动
    void randomMove();
    void startAttackAnimation();
private:
    void changeAnimation(BossAnimationType type);
    SDL_Rect getBossFrameRect();
    void startHorizontal(float left, float right);
    void startCircle(float radius);

    void updateBossAnimation(float deltaTime);
    void updateMove(float deltaTime);
    void updateMoveTo(float deltaTime);
    void updateHorizontal(float deltaTime);
    void updateCircle(float deltaTime);

    void renderBossAnimation(SDL_Renderer* renderer);

    void setRandomTargetPosition();

    BossMoveType moveType = BossMoveType::enter;
    BossType type = BossType::boss1;
    bool isDead = false;
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint targetPosition = {0.0f, 0.0f};
    float moveSpeed = 200.0f;
    int health = 4500;
    bool isFinish = false;
    int width = 64;
    int height = 80;
    SDL_RendererFlip isFlip = SDL_FLIP_NONE;
    Pattern* currentPattern = nullptr;
    int currentFrame = 0;
    int totalFrame = 0;
    Uint32 starTime = 0;
    Uint32 FPS = 15;
    BossAnimationType currentAnimationType = BossAnimationType::idle;
    bool firstMoveLoop = true;
    int loopStartFrame = 0;

    float leftLimit = 0.0f;
    float rightLimit = 0.0f;
    float horizontalDir = 0.0f;

    SDL_FPoint center = {0.0f, 0.0f}; //圆心
    SDL_FPoint lastPosition = {0.0f, 0.0f};
    float radius = 80.0f; //半径
    float angle = 0.0f; //当前角度
    float angularSpeed = 90.0f; //每秒90度
    
    //游玩区
    int play_w = 0;
    int play_h = 0;
    int margin = 32;

    bool stageDefeated = false;
};



#endif
