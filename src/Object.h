#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <vector>
#include <unordered_map>

enum class BulletType
{
    granBlue, //蓝色粒弹
    granRed
};

enum class PlayerAnimationType
{
    idle,
    left,
    right
};

enum class PlayerBulletType
{
    bullet1,
    bullet2
};

enum class BossAnimationType
{
    idle,
    left,
    right,
    attack
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

enum class SpawnType
{
    Enemy,
    Boss
};

enum class BossType
{
    boss1,
    COUNT
};

enum class BossMoveType
{
    enter, //出场进入
    idle, //固定位置
    moveto, //移动到一个点
    horizontal, //左右移动
    circle, //环形移动
    COUNT
};

enum class ItemType
{
    
};

enum class TitleButtonType
{
    start,
    option,
    quit,
    COUNT
};

enum class OptionButtonType
{
    vol,
    sevol,
    mode,
    quit,
    COUNT
};

enum class UiItemType
{
    hiScore,
    score,
    player,
    spell,
    health,
    bomb
};

struct Settings
{
    int vol = 100;
    int sevol = 100;
    bool mode = false;
};

struct OptionItem
{
    OptionButtonType type;
    SDL_Rect src;
    SDL_Rect dst;
    std::vector<OptionItem> variants;
    int currentVariant; //子选项索引
};

struct TitleItem
{
    TitleButtonType type;
    SDL_Rect src;
    SDL_Rect dst;
};

struct UiItem
{
    UiItemType type;
    SDL_Rect src;
    SDL_Rect dst;
    std::vector<UiItem> variants;
};

struct Player
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    int width = 32;
    int height = 48;
    int BaseSpeed = 400;
    Uint32 cooldown = 100;
    Uint32 lastShootTime = 0;
    int currentHealth = 5;
    int lastHealth = 5;
    const int maxHealth = 5;
    int currentFrame = 0;
    const int maxBomb = 3;
    int currentBomb = 3;
    Uint32 bombCooldown = 500;
    Uint32 lastBombTime = 0;
    int totalFrame = 0;
    Uint32 starTime = 0;
    Uint32 FPS = 15;
    PlayerAnimationType currentAnimationType = PlayerAnimationType::idle;
    PlayerBulletType currentBulletType = PlayerBulletType::bullet1;
    bool firstMoveLoop = true; //是否第一次循环
    int loopStartFrame = 0; //下一轮开始帧
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
    SDL_FPoint direction = {0.0f, -1.0f};
    PlayerBulletType type = PlayerBulletType::bullet1;
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