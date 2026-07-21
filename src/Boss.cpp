#include "Boss.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

bool Boss::init(SDL_Texture* texture, float x, float y)
{
    if(!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "boss Texture init Error");
        return false;
    }
    this->texture = texture;
    this->position.x = x;
    this->position.y = y;
    int texW = 0, texH = 0;
    SDL_QueryTexture(this->texture, nullptr, nullptr, &texW, &texH);
    this->totalFrame = texW / width;
    starTime = SDL_GetTicks();
    //startEnter(300.0f, 300.0f);
    startHorizontal(0.0f, 400.0f);
    return true;
}

void Boss::update(float deltaTime)
{
    updateMove(deltaTime);
    updateBossAnimation(deltaTime);
}

void Boss::render(SDL_Renderer *renderer)
{
    renderBossAnimation(renderer);
}

void Boss::takeDamage(int damage)
{
}

void Boss::changeAnimation(BossAnimationType type)
{
    if(currentAnimationType != type)
    {
        currentAnimationType = type;
        currentFrame = 0;
        starTime = SDL_GetTicks();
        firstMoveLoop = true;
        loopStartFrame = 0;
    }
}

SDL_Rect Boss::getBossFrameRect()
{
    int posY = 0;
    switch(currentAnimationType)
    {
        case BossAnimationType::idle:
            isFlip = SDL_FLIP_NONE;
            break;
        case BossAnimationType::left:
            posY = 1;
            isFlip = SDL_FLIP_HORIZONTAL;
            break;
        case BossAnimationType::right:
            posY = 1;
            isFlip = SDL_FLIP_NONE;
            break;
        case BossAnimationType::attack:
            break;
    }
    SDL_Rect src = {
        static_cast<int>(currentFrame * width),
        static_cast<int>(posY * height),
        static_cast<int>(width),
        static_cast<int>(height)
    };
    return src;
}

void Boss::startEnter(float x, float y)
{
    moveType = BossMoveType::enter;
    setTargetPosition(x, y);
}

void Boss::setTargetPosition(float x, float y)
{
    targetPosition = {x, y};
}

void Boss::startHorizontal(float left, float right)
{
    moveType = BossMoveType::horizontal;
    leftLimit = left;
    rightLimit = right;
    horizontalDir = 1.0f;
}

void Boss::updateBossAnimation(float deltaTime)
{
    Uint32 currentTime = SDL_GetTicks();
    currentFrame = loopStartFrame + ((currentTime - starTime) * FPS / 1000);
    if(currentFrame >= totalFrame)
    {
        starTime = currentTime;
        if (currentAnimationType == BossAnimationType::left ||
            currentAnimationType == BossAnimationType::right)
        {
            if(firstMoveLoop)
            {
                firstMoveLoop = false;
                loopStartFrame = 3;
            }
            currentFrame = loopStartFrame;
        }
        else
        {
            currentFrame = 0;
        }
    }
}

void Boss::updateMove(float deltaTime)
{
    switch(moveType)
    {
        case BossMoveType::enter:
            updateMoveTo(deltaTime);
            break;
        case BossMoveType::idle:
            break;
        case BossMoveType::horizontal:
            updateHorizontal(deltaTime);
            break;
    }
}

void Boss::updateMoveTo(float deltaTime)
{
    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    float length = sqrt(dx * dx + dy * dy);
    if(length < 2.0f) //判断是否到达目的地
    {
        position.x = targetPosition.x;
        position.y = targetPosition.y;
        moveType = BossMoveType::idle;
        changeAnimation(BossAnimationType::idle);
        return;
    }

    if(dx > 0)
    {
        changeAnimation(BossAnimationType::right);
    }
    else if(dx < 0)
    {
        changeAnimation(BossAnimationType::left);
    }

    dx /= length;
    dy /= length;
    position.x += dx * moveSpeed * deltaTime;
    position.y += dy * moveSpeed * deltaTime;
}

void Boss::updateHorizontal(float deltaTime)
{
    position.x += horizontalDir * moveSpeed * deltaTime;

    if(horizontalDir > 0)
    {
        changeAnimation(BossAnimationType::right);
    }
    else
    {
        changeAnimation(BossAnimationType::left);
    }

    if(position.x >= rightLimit)
    {
        position.x = rightLimit;
        horizontalDir = -1;
    }
    else if(position.x <= leftLimit)
    {
        position.x = leftLimit;
        horizontalDir = 1;
    }
}

void Boss::renderBossAnimation(SDL_Renderer *renderer)
{
    if(isDead)
    {
        return;
    }
    SDL_Rect src = getBossFrameRect();
    SDL_Rect dst = {
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(width * 1.5),
        static_cast<int>(height * 1.5)
    };
    SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, nullptr, isFlip);
}
