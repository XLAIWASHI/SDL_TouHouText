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
    Boss();
    ~Boss();
    bool init(SDL_Texture* texture, float x, float y);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void takeDamage(int damage); //收到伤害
    void startEnter(float x, float y);
private:
    void changeAnimation(BossAnimationType type);
    SDL_Rect getBossFrameRect();
    void setTargetPosition(float x, float y);
    void startHorizontal(float left, float right);

    void updateBossAnimation(float deltaTime);
    void updateMove(float deltaTime);
    void updateMoveTo(float deltaTime);
    void updateHorizontal(float deltaTime);

    void renderBossAnimation(SDL_Renderer* renderer);

    
    bool isDead = false;
    BossMoveType moveType = BossMoveType::enter;

    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint targetPosition = {0.0f, 0.0f};
    float moveSpeed = 200.0f;
    int health = 100;
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
};



#endif