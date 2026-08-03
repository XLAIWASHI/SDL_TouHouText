#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include "Boss.h"
#include <unordered_map>
#include <list>
#include <string>
#include <random>
#include <vector>
#include <fstream>

class EnemyBullet;
class BulletPattern;
class BulletManager;
class BossFightController;
class Boss1Fight;

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

    //碰撞检测
    bool ColliderEnemies(Enemy* enemy);
    void ColliderBossBullet();
    void ColliderBoss();
    void enemyExplode(Enemy* enemy);

    //玩家减血
    void playerTakeDamage(int damage);

    //发射子弹
    void shootPlayer();
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
    

    int margin = 32;

    Background background;
    PlayArea playarea;

    Player player;
    PlayerPoint playerPoint;
    bool isDead = false;//玩家是否死亡
    bool invincible = false;//玩家是否无敌
    float invincibleTimer = 0.0f;
    Enemy enemyTemplate;

    //容器相关
    std::unordered_map<std::string, SDL_Texture*> BulletTextureManager;//子弹纹理库
    std::unordered_map<std::string, SDL_Texture*> BossTextureManager;//boss纹理库
    std::list<PlayerBullet*> PlayerBullets;//玩家子弹库
    std::list<Enemy*> Enemies;//敌人库
    std::list<EnemyBullet*> EnemiesBullets;//敌人子弹库
    std::list<EnemyBullet*>* BossBullets;

    //随机数相关
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    //UI相关
    SDL_Texture* uiHealth = nullptr;

    //Boss相关
    Boss* boss = nullptr;
    BulletManager* bulletManager = nullptr;
    
    BossFightController* bossFightController = nullptr;

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