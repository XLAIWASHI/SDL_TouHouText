#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include "Boss.h"
#include "Pseudo3DBackground.h"
#include <unordered_map>
#include <list>
#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class EnemyBullet;
class BulletPattern;
class BulletManager;
class BossFightController;
class Boss1Fight;
class EffectManager;
class Bomb;

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

private:
    //更新相关
    void keyboardControl(float deltaTime);
    void updatePlayArea(float deltaTime);
    void updatePlayerAnimation(float deltaTime);
    void updatePlayerPoint(float deltaTime);
    void updatePlayer(float deltaTime);
    void updateEnemies(float deltaTime);
    void updatePlayerBullet(float deltaTime);
    void updateEnemiesAnimation(float deltaTime, Enemy* enemy);
    void updateEnemiesBullet(float deltaTime);
    void updateWave(float deltaTime);//更新波次表
    void updateBomb(float deltaTime);
    void updateItems(float deltaTime);

    //渲染相关
    void renderBackground();
    void renderPlayArea();
    void renderPlayerAnimation();
    void renderPlayerPoint();
    void renderPlayerBullet();
    void renderPlayer();
    void renderEnemies();
    void renderEnemiesAnimation();
    void renderEnemiesBullet();
    void renderUI();
    void renderBossUI();
    void renderPlayerUI();
    void renderScore(int value, int x, int y);
    void renderDigit(int digit, int x, int y);
    void renderBomb();
    void renderItems();

    //碰撞检测
    bool ColliderEnemies(Enemy* enemy);
    void ColliderBossBullet();
    void ColliderBoss();
    void ColliderBomb();
    bool ColliderItems(Item* item);

    void enemyExplode(Enemy* enemy);

    //玩家减血
    void playerTakeDamage(int damage);

    //发射子弹
    void shootPlayer();
    void shootBomb();
    void shootEnemy(Enemy* enemy, SDL_FPoint offset);
    
    //生成敌人
    void spawnEnemyAtType(EnemyType type, float x, float y, int dirX, int dirY);
    void spawnBoss(BossType type, float x, float y);
    //设置敌人当前状态的总帧数
    void setEnemyTotalFrame(Enemy* enemy);
    SDL_Rect getEnemyFrameRect(Enemy* enemy);
    
    //获得角度
    SDL_FPoint getDirection(Enemy* enemy, EnemyBullet* bullet);

    //波次相关
    void loadWavesFromFile(const std::string& filename);
    
    //得分相关
    void addScore(int value);
    void addBomb(int value);
    void addLife(int value);

    //读取文件
    void loadSceneData(const std::string& filename);
    void loadUI(const json& data);
    void loadItem(const json& data);
    void loadBackground(const json& dataBackground, const json& dataWorld);

    //游戏结束
    void gameOver();

    //生成item
    void dropItem(SDL_FPoint position);

    //分数相关
    int score = 0;
    std::vector<UiItem> uiItems;
    TTF_Font* scoreFont = nullptr;
    int ui_offset_startX = 0;
    int ui_offset_startY = 0;
    float ui_mult = 0.0f;
    int offset = 0;
    SDL_Texture* ui_playerTexture = nullptr;
    SDL_Texture* ui_ascii = nullptr;
    int scoreStartX = 0;
    int scoreStartY = 0;
    int scoreWidth = 0;
    int scoreHeight = 0;

    //item相关
    SDL_Texture* itemsTexture = nullptr;
    SDL_Rect pointSrc;
    SDL_Rect powerSrc;
    SDL_Rect bombSrc;
    SDL_Rect lifeSrc;

    //Bomb
    SDL_Texture* bombTexture = nullptr;
    bool isBomb = false;
    Bomb* bomb = nullptr;


    int margin = 32;

    Background background;
    PlayArea playarea;

    // === AI 实现：伪3D背景 ===
    Pseudo3DBackground pseudo3DBg;

    Player player;
    PlayerPoint playerPoint;
    Enemy* playerBulletTarget = nullptr;
    Boss* playerBulletBossTarget = nullptr;
    bool isDead = false;//玩家是否死亡
    bool invincible = false;//玩家是否无敌
    float invincibleTimer = 0.0f;
    bool isDeadInterval = false;//死亡间隔是否开始
    float deadIntervalTime = 0.0f;
    float deadTimer = 0.0f;
    Enemy enemyTemplate;

    //容器相关
    std::unordered_map<std::string, SDL_Texture*> BulletTextureManager;//子弹纹理库
    std::unordered_map<std::string, SDL_Texture*> BossTextureManager;//boss纹理库
    std::list<PlayerBullet*> PlayerBullets;//玩家子弹库
    std::list<Enemy*> Enemies;//敌人库
    std::list<EnemyBullet*> EnemiesBullets;//敌人子弹库
    std::list<EnemyBullet*>* BossBullets = nullptr;
    std::list<Item*> Items;

    //随机数相关
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    //Boss相关
    Boss* boss = nullptr;
    BulletManager* bulletManager = nullptr;
    
    BossFightController* bossFightController = nullptr;

    //特效相关
    EffectManager* effectManager = nullptr;

    //波次表
    struct SpawnCmd
    {
        SpawnType spawnType;
        EnemyType enemyType;
        BossType bossType;
        float delay;
        float posX;
        float posY;
        int dirX;
        int dirY;
    };

    struct Wave
    {
        std::vector<SpawnCmd> spawns;
        float duration;
        bool waitClear;
    };

    std::vector<Wave> waves;//所有波次
    int curWave = 0;//当前波次
    float waveTimer = 0.0f;
    int nextSpawnIdx = 0;
};

#endif