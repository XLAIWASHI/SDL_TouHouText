#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>

enum class PlayerAnimationType
{
    idle,
    left,
    right
};

enum class PlayerBulletType
{
    bullet1,
    bullet2,
    bullet3,
    COUNT
};

enum class EnemyType
{
    enemyBase1,
    enemyBase2,
    enemyBase3,
    enemyBase4,
    COUNT
};

enum class EnemyAnimationType
{
    right,
    left,
    down,
    COUNT
};

enum class ItemType
{
    
};

enum class TitleButtonType
{
    star,
    option,
    quit,
    COUNT
};

struct Player
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    int width = 32;
    int height = 48;
    int BaseSpeed = 400;
    Uint32 cooldown = 50;
    Uint32 lastShootTime = 0;
    int currentHealth = 10;
    const int maxHealth = 10;
    int currentFrame = 0;
    int totalFrame = 0;
    Uint32 starTime = 0;
    Uint32 FPS = 15;
    PlayerAnimationType currentAnimationType = PlayerAnimationType::idle;
    PlayerBulletType currentBulletType = PlayerBulletType::bullet1;
};

struct PlayerPoint
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    int w;
    int h;
    
};

struct PlayerBullet
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    int width = 0;
    int height = 0;
    int speed = 600;
    int damage = 1;
};

struct Enemy
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint direction = {0.0f, 0.0f};
    int width = 32;
    int height = 32;
    int speed = 200;
    Uint32 cooldown = 2000;
    Uint32 lastShoot = 0;
    int health = 5;
    int currentFrame = 0;
    int totalFrame = 0;
    SDL_RendererFlip isFlip = SDL_FLIP_NONE;//none = 0不反转;horizontal = 1左右反转;vertical = 2上下反转;
    Uint32 starTime = 0;
    Uint32 FPS = 15;
    EnemyType currentEnemyType = EnemyType::enemyBase1;
    EnemyAnimationType currentAnimationType = EnemyAnimationType::down;
};

struct EnemyBullet
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint direction = {0.0f, 0.0f};
    int width = 0;
    int height = 0;
    int speed = 200;
    int damage = 1;
    float delay = 1.0f;
    float timer = 0.0f;
    float lifeTimer = 0.0f;//已经存活的时间
    const float lifeTime = 6.0f;
    EnemyType type = EnemyType::enemyBase1;//谁的子弹
};

struct Background
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    int width = 0;
    int height = 0;
};

struct Title
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    int width = 0;
    int height = 0;
};

struct PlayArea
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    float offset = 0.0f;
    int width = 0;
    int height = 0;
    int speed = 30;
};

struct Item
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint direction = {0.0f, 0.0f};
    int w = 0;
    int h = 0;
    int speed = 400;


};

#endif