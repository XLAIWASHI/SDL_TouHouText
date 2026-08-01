#include "SceneMain.h"
#include "SceneTitle.h"
#include "Game.h"
#include "EnemyBullet.h"
#include "BulletPattern.h"
#include "BulletManager.h"
#include "BossFightController.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    game.playBGM("assets\\music\\bgm\\th08_09.mid");
    //导入波次表
    loadWavesFromFile("data\\waves.json");
    //随机数
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);
    //背景资源导入
    background.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\backgroundUI\\backgroundUI.png");
    if(background.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "backGround init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(background.texture, nullptr, nullptr, &background.width, &background.height);
    //游玩区资源导入
    playarea.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\playarea\\playarea.png");
    if(playarea.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "playArea init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(playarea.texture, nullptr, nullptr, &playarea.width, &playarea.height);
    playarea.width = game.getPlayAreaWidth();
    //玩家资源导入
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\player\\pl00.png");
    if(player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "player init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    int texW = 0, texH = 0;
    SDL_QueryTexture(player.texture, nullptr, nullptr, &texW, &texH);
    player.totalFrame = texW / player.width;
    player.width *= 1.5;
    player.height *= 1.5;
    player.position.x = margin + game.getPlayAreaWidth() / 2 - player.width / 2;
    player.position.y = margin + game.getPlayAreaHeight() - player.height;
    //玩家判定点
    playerPoint.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\player\\pl00b.png");
    if(playerPoint.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "player point init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(playerPoint.texture, nullptr, nullptr, &playerPoint.w, &playerPoint.h); 
    playerPoint.w /= 4;
    playerPoint.h /= 4;
    //开启混合模式并设置判定点透明度
    SDL_SetTextureBlendMode(playerPoint.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(playerPoint.texture, 0);//透明
    //子弹资源加载
    BulletTextureManager["bullet1"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\札弹\\札弹1.png");
    BulletTextureManager["bullet2"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\札弹\\札弹8.png");
    BulletTextureManager["bullet3"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\札弹\\札弹11.png");
    BulletTextureManager["EnemyBullet1"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹210.png");
    BulletTextureManager["EnemyBullet2"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹0.png");
    BulletTextureManager["EnemyBullet3"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹30.png");
    BulletTextureManager["EnemyBullet4"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹300.png");
    
    bulletManager = new BulletManager(margin, game.getPlayAreaWidth(), game.getPlayAreaHeight());

    for(auto& bullet : BulletTextureManager)
    {
        if(bullet.second == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "bullet init Error: %s\n", SDL_GetError());
            game.getIsRunning() = false;
            return;
        }
    }
    //Boss资源加载
    BossTextureManager["boss1"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\boss\\stg3enm.png");
    for(auto& boss : BossTextureManager)
    {
        if(boss.second == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "boss init Error: %s\n", SDL_GetError());
            game.getIsRunning() = false;
            return;
        }
    }
    //敌人资源加载
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\enemy\\enemy5.png");
    if(enemyTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Enemy init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    //UI相关
    uiHealth = IMG_LoadTexture(game.getRenderer(), "assets\\image\\UI\\Health UI Black.png");
    if(uiHealth == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UI init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
}

void SceneMain::handleEvent(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            SceneTitle* sceneTitle = new SceneTitle();
            game.changeScene(sceneTitle);
        }
    }
}

void SceneMain::update(float deltaTime)
{
    //按键控制
    keyboardControl(deltaTime);
    //更新游玩区
    updatePlayArea(deltaTime);
    //更新玩家
    updatePlayer(deltaTime);
    //更新敌人
    updateEnemies(deltaTime);
    //更新玩家子弹
    updatePlayerBullet(deltaTime);
    //更新敌人子弹
    updateEnemiesBullet(deltaTime);
    bulletManager->update(deltaTime);
    if(boss != nullptr)
    {
        boss->update(deltaTime);
        ColliderBossBullet();
        if(!boss->isBossDead())
        {
            ColliderBoss();
            if(bossFightController != nullptr)
                bossFightController->update(deltaTime, *bulletManager, {player.position.x + player.width / 2, player.position.y + player.height / 2});
        }
        else if(bulletManager->isEmpty())
        {
            delete boss;
            boss = nullptr;
            if(bossFightController != nullptr)
            {
                delete bossFightController;
                bossFightController = nullptr;
            }
        }
    }
    //生成敌人
    updateWave(deltaTime);
    
}

void SceneMain::render()
{
    //渲染背景
    renderBackground();
    //渲染游玩区
    renderPlayArea();
    //渲染敌人
    renderEnemies();
    if(boss != nullptr)
    {
        boss->render(game.getRenderer());
    }
    //渲染玩家
    renderPlayer();
    //渲染敌人子弹
    renderEnemiesBullet();
    bulletManager->render(game.getRenderer());
    //渲染玩家子弹
    renderPlayerBullet();
    //渲染UI
    renderUI();
}

void SceneMain::clean()
{
    //背景
    if(background.texture != nullptr)
    {
        SDL_DestroyTexture(background.texture);
    }
    //游玩区
    if(playarea.texture != nullptr)
    {
        SDL_DestroyTexture(playarea.texture);
    }
    //UI相关
    if(uiHealth != nullptr)
    {
        SDL_DestroyTexture(uiHealth);
    }
    //玩家
    if(player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
    //玩家判定点
    if(playerPoint.texture != nullptr)
    {
        SDL_DestroyTexture(playerPoint.texture);
    }
    //敌人
    if(enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    //子弹纹理库
    for(auto& bullet : BulletTextureManager)
    {
        if(bullet.second != nullptr)
        {
            SDL_DestroyTexture(bullet.second);
        }
    }
    BulletTextureManager.clear();
    //boss纹理库
    for(auto& bs : BossTextureManager)
    {
        if(bs.second != nullptr)
        {
            SDL_DestroyTexture(bs.second);
        }
    }
    BossTextureManager.clear();
    //玩家子弹库
    for(auto& bullet : PlayerBullets)
    {
        if(bullet != nullptr)
        {
            delete bullet;
        }
    }
    PlayerBullets.clear();
    //敌人库
    for(auto& enemy : Enemies)
    {
        if(enemy != nullptr)
        {
            delete enemy;
        }
    }
    Enemies.clear();

}

void SceneMain::renderBackground()
{
    for(int posY = 0; posY < game.getWindowHeight(); posY += background.height)
    {
        for(int posX = 0; posX < game.getWindowWidth(); posX += background.width)
        {
            SDL_Rect rect = {posX, posY, background.width, background.height};
            SDL_RenderCopy(game.getRenderer(), background.texture, nullptr, &rect);
        }
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    if(isDead)
    {
        return;
    }
    int currentSpeed = player.BaseSpeed;
    Uint32 currentTime = SDL_GetTicks();
    auto keyboardState = SDL_GetKeyboardState(nullptr);

    bool isMoving = false;
    SDL_SetTextureAlphaMod(playerPoint.texture, 0);//透明
    SDL_SetTextureColorMod(player.texture, 255, 255, 255);
    //低速移动
    if(keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT])
    {
        currentSpeed = player.BaseSpeed / 2;
        SDL_SetTextureAlphaMod(playerPoint.texture, 255);//不透明
    }

    PlayerAnimationType oldType = player.currentAnimationType;
    PlayerAnimationType newType = oldType;

    if(keyboardState[SDL_SCANCODE_W])
    {
        isMoving = true;
        player.position.y -= deltaTime * currentSpeed;
        newType = PlayerAnimationType::idle;
    }
    if(keyboardState[SDL_SCANCODE_S])
    {
        isMoving = true;
        player.position.y += deltaTime * currentSpeed;
        newType = PlayerAnimationType::idle;
    }
    if(keyboardState[SDL_SCANCODE_A])
    {
        isMoving = true;
        player.position.x -= deltaTime * currentSpeed;
        newType = PlayerAnimationType::left;
    }
    if(keyboardState[SDL_SCANCODE_D])
    {
        isMoving = true;
        player.position.x += deltaTime * currentSpeed;
        newType = PlayerAnimationType::right;
    }
    
    //如果动画类型发生了改变，重置计时器和帧
    if(newType != oldType)
    {
        player.currentAnimationType = newType;
        player.starTime = currentTime;
        player.currentFrame = 0;

        //左右动画重新开始
        if (newType == PlayerAnimationType::left ||
            newType == PlayerAnimationType::right)
        {
            player.firstMoveLoop = true;
            player.loopStartFrame = 0;
        }
    }

    if (!isMoving)
    {
        //松开恢复
        if(player.currentAnimationType != PlayerAnimationType::idle)
        {
            player.currentAnimationType = PlayerAnimationType::idle;
            player.starTime = currentTime;
            player.currentFrame = 0;

            //下次再按左右重新播放动画
            player.firstMoveLoop = true;
            player.loopStartFrame = 0;
        }
    }

    //限制边界
    if(player.position.x < margin)
    {
        player.position.x = margin;
    }
    if(player.position.x > game.getPlayAreaWidth() + margin - player.width)
    {
        player.position.x = game.getPlayAreaWidth() + margin - player.width;
    }
    if(player.position.y < margin)
    {
        player.position.y = margin;
    }
    if(player.position.y > game.getPlayAreaHeight() + margin - player.height)
    {
        player.position.y = game.getPlayAreaHeight() + margin - player.height;
    }

    //是否发射
    if(keyboardState[SDL_SCANCODE_J])
    {
        if(currentTime - player.lastShootTime > player.cooldown)
        {
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }

    //切换弹幕类型
    if(!isPressed && keyboardState[SDL_SCANCODE_I])
    {
        int temp = (static_cast<int>(player.currentBulletType) + 1) % static_cast<int>(PlayerBulletType::COUNT);
        player.currentBulletType = (PlayerBulletType)temp;
        isPressed = true;
    }
    else if(!keyboardState[SDL_SCANCODE_I])
    {
        isPressed = false;
    }
}

void SceneMain::updatePlayArea(float deltaTime)
{
    playarea.offset += playarea.speed * deltaTime;
    if(playarea.offset > 0)
    {
        playarea.offset -= playarea.height;
    }
}

void SceneMain::updatePlayerAnimation(float deltaTime)
{
    Uint32 currentTime = SDL_GetTicks();
    player.currentFrame = player.loopStartFrame + ((currentTime - player.starTime) * player.FPS / 1000);
    if(player.currentFrame >= player.totalFrame)
    {
        player.starTime = currentTime;
        
        if (player.currentAnimationType == PlayerAnimationType::left ||
            player.currentAnimationType == PlayerAnimationType::right)
        {
            if(player.firstMoveLoop)
            {
                //第一轮结束，之后从第3帧开始
                player.firstMoveLoop = false;
                player.loopStartFrame = 3;
            }
            player.currentFrame = player.loopStartFrame;
        }
        else
        {
            player.currentFrame = 0;
        }
    }
}

void SceneMain::updatePlayerPoint(float deltaTime)
{
    playerPoint.position.x = player.position.x + player.width / 2 - playerPoint.w / 2;
    playerPoint.position.y = player.position.y + player.height / 2 - playerPoint.h / 2;
}

void SceneMain::updatePlayer(float deltaTime)
{
    //更新玩家动画
    updatePlayerAnimation(deltaTime);
    //更新玩家判定点
    updatePlayerPoint(deltaTime);
    if(isDead)
    {
        return;
    }
    if(invincible)
    {
        invincibleTimer += deltaTime;
        if(invincibleTimer >= 1.0f)
        {
            invincibleTimer = 0.0f;
            invincible = false;
        }
    }
    if(player.currentHealth <= 0)
    {
        SDL_SetTextureAlphaMod(playerPoint.texture, 0);//判定点透明
        isDead = true;
    }
}

void SceneMain::updateEnemies(float deltaTime)
{
    for(auto it = Enemies.begin(); it != Enemies.end(); )
    {
        Enemy* enemy = *it;
        //更新敌人动画
        updateEnemiesAnimation(deltaTime, enemy);
        //更新位置
        enemy->position.x += enemy->speed * deltaTime * enemy->direction.x;
        enemy->position.y += enemy->speed * deltaTime * enemy->direction.y;
        //判断是否发射子弹
        Uint32 currentTime = SDL_GetTicks();
        if(!isDead && (currentTime - enemy->lastShoot) > enemy->cooldown)
        {
            if(enemy->currentEnemyType == EnemyType::enemyBase1)
            {
                shootEnemy(enemy, {0, 1});
            }
            else if(enemy->currentEnemyType == EnemyType::enemyBase2)
            {
                shootEnemy(enemy, {-0.4f, 1});
                shootEnemy(enemy, {0, 1});
                shootEnemy(enemy, {0.4f, 1});
            }
            else if(enemy->currentEnemyType == EnemyType::enemyBase3)
            {
                shootEnemy(enemy, getDirection(enemy, nullptr));
            }
            else if(enemy->currentEnemyType == EnemyType::enemyBase4)
            {
                shootEnemy(enemy, getDirection(enemy, nullptr));
            }
            enemy->lastShoot = currentTime;   
        }
        //碰撞检测
        if(ColliderEnemies(enemy))
        {
            it = Enemies.erase(it);
        }
        else
        {
            if(enemy->health <= 0)
            {
                enemyExplode(enemy);
                it = Enemies.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
    
}

void SceneMain::updatePlayerBullet(float deltaTime)
{

    
    for(auto it = PlayerBullets.begin(); it != PlayerBullets.end(); )
    {
        PlayerBullet* bullet = *it;
        bullet->position.y -= bullet->speed * deltaTime;
        if(bullet->position.y < margin)
        {
            delete bullet;
            it = PlayerBullets.erase(it);
        }
        else
        {
            bool hit = false;
             SDL_Rect bulletRect = {
                static_cast<int>(bullet->position.x),
                static_cast<int>(bullet->position.y),
                bullet->width,
                bullet->height
             };
            for(Enemy* enemy : Enemies)
            {
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                if(SDL_HasIntersection(&bulletRect, &enemyRect))
                {
                    enemy->health -= bullet->damage;
                    delete bullet;
                    it = PlayerBullets.erase(it);
                    hit = true;
                    break;
                }
            }
            if(!hit && boss != nullptr)
            {
                SDL_Rect bossRect = {
                    static_cast<int>(boss->getBossPos().x),
                    static_cast<int>(boss->getBossPos().y),
                    boss->getBossWidth(),
                    boss->getBossHeight()
                };
                if(SDL_HasIntersection(&bulletRect, &bossRect))
                {
                    boss->takeDamage(bullet->damage);
                    delete bullet;
                    it = PlayerBullets.erase(it);
                    hit = true;
                }
            }
            if(!hit)
            {
                it++;
            }
        }
    }
}

void SceneMain::updateEnemiesAnimation(float deltaTime, Enemy* enemy)
{
    Uint32 currentTime = SDL_GetTicks();
    
    enemy->currentFrame = (currentTime - enemy->starTime) * enemy->FPS / 1000.0f;
    if(enemy->currentFrame >= enemy->totalFrame)
    {
        enemy->starTime = currentTime;
        enemy->currentFrame = 0;
    }
    
}

void SceneMain::updateEnemiesBullet(float deltaTime)
{
    for(auto it = EnemiesBullets.begin(); it != EnemiesBullets.end(); )
    {
        EnemyBullet* bullet = *it;
        bullet->timer += deltaTime;

        if(bullet->enemyType == EnemyType::enemyBase4)
        {
            bullet->lifeTimer += deltaTime;
            //得到目标方向
            SDL_FPoint targetDir = getDirection(nullptr, bullet);
            //平滑转向
            float smooth = 0.05f;
            bullet->direction.x += (targetDir.x - bullet->direction.x) * smooth;
            bullet->direction.y += (targetDir.y - bullet->direction.y) * smooth;
            //归一化
            float len = sqrt(bullet->direction.x * bullet->direction.x + bullet->direction.y * bullet->direction.y);
            bullet->direction.x /= len;
            bullet->direction.y /= len;
            //和获得方向是一样的，就是比直接获得方向多了一个 顺滑度，普通的时候不是 * 1嘛
            //就是手动的让他更慢一点，方便玩家躲闪，之后还是和正常的方向一样，需要归一化
            
            //判断生存时间
            if(bullet->lifeTimer > bullet->lifeTime)
            {
                delete bullet;
                it = EnemiesBullets.erase(it);
                continue;
            }
        }
        bullet->position.x += bullet->speed * bullet->direction.x * deltaTime;
        bullet->position.y += bullet->speed * bullet->direction.y * deltaTime;

        //判断是否超界
        if( bullet->position.y > margin + game.getPlayAreaHeight() - bullet->height ||
            bullet->position.y < margin ||
            bullet->position.x < margin ||
            bullet->position.x > margin + game.getPlayAreaWidth() - bullet->width)
            {
                delete bullet;
                it = EnemiesBullets.erase(it);
            }
            else
            {
                SDL_Rect playerPointRect = {
                    static_cast<int>(playerPoint.position.x),
                    static_cast<int>(playerPoint.position.y),
                    playerPoint.w,
                    playerPoint.h
                };
                SDL_Rect bulletRect = {
                    static_cast<int>(bullet->position.x),
                    static_cast<int>(bullet->position.y),
                    bullet->width,
                    bullet->height
                };
                if(SDL_HasIntersection(&playerPointRect, &bulletRect))
                {
                    playerTakeDamage(bullet->damage);
                    delete bullet;
                    it = EnemiesBullets.erase(it);

                }
                else
                {
                    it++;
                }
            }
        
    }
}

void SceneMain::shootPlayer()
{
    PlayerBullet* bullet1 = new PlayerBullet();
    PlayerBullet* bullet2 = new PlayerBullet();
    if(player.currentBulletType == PlayerBulletType::bullet1)
    {
        bullet1->texture = BulletTextureManager["bullet1"];
        bullet2->texture = BulletTextureManager["bullet1"];
    }
    else if(player.currentBulletType == PlayerBulletType::bullet2)
    {
        bullet1->texture = BulletTextureManager["bullet2"];
        bullet2->texture = BulletTextureManager["bullet2"];
    }
    else if(player.currentBulletType == PlayerBulletType::bullet3)
    {
        bullet1->texture = BulletTextureManager["bullet3"];
        bullet2->texture = BulletTextureManager["bullet3"];
    }
    SDL_QueryTexture(bullet1->texture, nullptr, nullptr, &bullet1->width, &bullet1->height);
    bullet1->width *= 1.5, bullet1->height *= 1.5;
    bullet2->width = bullet1->width, bullet2->height = bullet1->height;

    bullet1->position.x = player.position.x;
    bullet1->position.y = player.position.y + player.height / 2 - bullet1->height / 2;

    bullet2->position.x = player.position.x + player.width - bullet2->width;
    bullet2->position.y = player.position.y + player.height / 2 - bullet1->height / 2;
    
    PlayerBullets.push_back(bullet1);
    PlayerBullets.push_back(bullet2);
}

void SceneMain::shootEnemy(Enemy* enemy, SDL_FPoint offset)
{
    EnemyBullet* bullet = new EnemyBullet();
    if(enemy->currentEnemyType == EnemyType::enemyBase1)
    {
        bullet->texture = BulletTextureManager["EnemyBullet1"];
        bullet->enemyType = EnemyType::enemyBase1;
    }
    else if(enemy->currentEnemyType == EnemyType::enemyBase2)
    {
        bullet->texture = BulletTextureManager["EnemyBullet2"];
        bullet->enemyType = EnemyType::enemyBase2;
    }
    else if(enemy->currentEnemyType == EnemyType::enemyBase3)
    {
        bullet->texture = BulletTextureManager["EnemyBullet3"];
        bullet->enemyType = EnemyType::enemyBase3;
    }
    else if(enemy->currentEnemyType == EnemyType::enemyBase4)
    {
        bullet->texture = BulletTextureManager["EnemyBullet4"];
        bullet->enemyType = EnemyType::enemyBase4;
    }
    SDL_QueryTexture(bullet->texture, nullptr, nullptr, &bullet->width, &bullet->height);
    bullet->width *= 2, bullet->height *= 2;
    bullet->position.x = enemy->position.x + enemy->width / 2 - bullet->width / 2;
    bullet->position.y = enemy->position.y + enemy->height / 2 - bullet->height / 2;
    bullet->direction.x = offset.x;
    bullet->direction.y = offset.y;
    EnemiesBullets.push_back(bullet);
}

void SceneMain::setEnemyTotalFrame(Enemy *enemy)
{
    if(enemy->currentAnimationType == EnemyAnimationType::right)
    {
        enemy->totalFrame = 7;
    }
    else if(enemy->currentAnimationType == EnemyAnimationType::left)
    {
        enemy->totalFrame = 7;
    }
    else if(enemy->currentAnimationType == EnemyAnimationType::down)
    {
        enemy->totalFrame = 5;
    }
}

SDL_Rect SceneMain::getEnemyFrameRect(Enemy *enemy)
{
    int posX = 0;
    if(enemy->currentAnimationType == EnemyAnimationType::left)
    {
        posX = 5;
        enemy->isFlip = SDL_FLIP_HORIZONTAL;
    }
    else if(enemy->currentAnimationType == EnemyAnimationType::right)
    {
        posX = 5;
        enemy->isFlip = SDL_FLIP_NONE;
    }
    else if(enemy->currentAnimationType == EnemyAnimationType::down)
    {
        enemy->isFlip = SDL_FLIP_NONE;
    }
    SDL_Rect src = {
        static_cast<int>((posX + enemy->currentFrame) * enemy->width / 1.3),
        static_cast<int>(static_cast<int>(enemy->currentEnemyType) * enemy->height / 1.3),
        static_cast<int>(enemy->width / 1.3),
        static_cast<int>(enemy->height / 1.3)
    };    

    return src;
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy, EnemyBullet* bullet)
{
    float x, y;
    if(bullet == nullptr)
    {
        x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
        y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    }
    else
    {
        x = (player.position.x + player.width / 2) - bullet->position.x;
        y = (player.position.y + player.height / 2) - bullet->position.y;
    }
    
    auto lenth = std::sqrt(x * x + y * y);
    x /= lenth;
    y /= lenth;

    return SDL_FPoint{x, y};
}

void SceneMain::loadWavesFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Open Waves Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    json data;
    file >> data;

    waves.clear();
    for(auto& wj : data["waves"])
    {
        Wave w;
        w.duration = wj.value("duration", 0.0f);//如果存在，拿第一个参数，不存在拿第二个
        w.waitClear = wj.value("waitClear", false);
        for(auto& sj : wj["spawns"])
        {
            SpawnCmd cmd;
            std::string typeStr = sj["type"];
            if(typeStr == "enemy")
            {
                cmd.spawnType = SpawnType::Enemy;
                std::string enemyStr = sj["enemyType"];
                if(enemyStr == "enemyBase1")
                {
                    cmd.enemyType = EnemyType::enemyBase1;
                }
                else if(enemyStr == "enemyBase2")
                {
                    cmd.enemyType = EnemyType::enemyBase2;
                }
                else if(enemyStr == "enemyBase3")
                {
                    cmd.enemyType = EnemyType::enemyBase3;
                }
                else if(enemyStr == "enemyBase4")
                {
                    cmd.enemyType = EnemyType::enemyBase4;
                }
            }
            else if(typeStr == "boss")
            {
                cmd.spawnType = SpawnType::Boss;
                std::string bossStr = sj["bossType"];
                if(bossStr == "boss1")
                {
                    cmd.bossType = BossType::boss1;
                }
            }
            
            cmd.delay = sj["delay"];
            cmd.posX = sj.value("posX", 0.5f);
            cmd.posY = sj.value("posY", 0.0f);
            cmd.dirX = sj.value("dirX", 0);
            cmd.dirY = sj.value("dirY", 1);
            w.spawns.push_back(cmd);
        }
        waves.push_back(w);
    }

    //重置状态
    curWave = 0;
    waveTimer = 0.0f;
    nextSpawnIdx = 0;
}

void SceneMain::updateWave(float deltaTime) {
    if (curWave >= waves.size()) return;   // 所有波次结束

    Wave& wave = waves[curWave];
    waveTimer += deltaTime;

    // 生成敌人
    while(nextSpawnIdx < wave.spawns.size() && waveTimer >= wave.spawns[nextSpawnIdx].delay)
    {
        SpawnCmd& cmd = wave.spawns[nextSpawnIdx];
        float realX = margin + cmd.posX * game.getPlayAreaWidth();
        float realY = margin + cmd.posY * game.getPlayAreaHeight();
        if(cmd.spawnType == SpawnType::Enemy)
        {
            spawnEnemyAtType(cmd.enemyType, realX, realY, cmd.dirX, cmd.dirY);
        }
        else if(cmd.spawnType == SpawnType::Boss)
        {
            spawnBoss(cmd.bossType, realX, realY);
            bossFightController = new BossFightController(BulletTextureManager);
            bossFightController->createFight(boss->getBossType(), boss);
        }
        nextSpawnIdx++;
    }

    // 判断本波是否结束
    bool finished = false;
    if(wave.waitClear)
    {
        if(nextSpawnIdx >= wave.spawns.size() && Enemies.empty() && boss == nullptr)
        {
            finished = true;
        }
    }
    else
    {
        if(wave.duration > 0 && waveTimer >= wave.duration)
        {
            finished = true;
        }
    }

    if(finished)
    {
        curWave++;
        waveTimer = 0.0f;
        nextSpawnIdx = 0;
    }
}

void SceneMain::spawnEnemyAtType(EnemyType type, float x, float y, int dirX, int dirY)
{
    Enemy* enemy = new Enemy(enemyTemplate);
    enemy->currentAnimationType = EnemyAnimationType::down;   // 默认向下动画
    enemy->width *= 1.3, enemy->height *= 1.3;
    setEnemyTotalFrame(enemy);
    enemy->starTime = SDL_GetTicks();
    enemy->position.x = x - enemy->width;//敌人命令那块坐标有点问题，这里减去一个宽度就行了
    enemy->position.y = y;//不用减，上面是对的，下面不用整
    float len = sqrt(dirX * dirX + dirY * dirY);
    if(len > 0.001f)
    {
        enemy->direction.x = dirX / len;
        enemy->direction.y = dirY / len;
    }
    else
    {
        enemy->direction.x = 0;
        enemy->direction.y = 1;
    }
    enemy->currentEnemyType = type;

    // 根据类型设置属性（速度、血量、CD等）
    switch(type)
    {
        case EnemyType::enemyBase1:
            enemy->speed = 140.0f; enemy->health = 1; enemy->cooldown = 1800; break;
        case EnemyType::enemyBase2:
            enemy->speed = 100.0f; enemy->health = 2; enemy->cooldown = 1800; break;
        case EnemyType::enemyBase3:
            enemy->speed = 130.0f; enemy->health = 3; enemy->cooldown = 1800; break;
        case EnemyType::enemyBase4:
            enemy->speed = 120.0f; enemy->health = 4; enemy->cooldown = 1800; break;
        default: break;
    }
    setEnemyTotalFrame(enemy);
    Enemies.push_back(enemy);
}
void SceneMain::spawnBoss(BossType type, float x, float y)
{
    if(boss != nullptr)
    {
        return;
    }
    boss = new Boss(game.getPlayAreaWidth(), game.getPlayAreaHeight(), margin);
    if(!boss->init(BossTextureManager["boss1"], x, y))
    {
        delete boss;
        boss = nullptr;
    }
}
void SceneMain::renderPlayArea()
{
    SDL_Rect clip = {margin, margin, game.getPlayAreaWidth(), game.getPlayAreaHeight()};
    SDL_RenderSetClipRect(game.getRenderer(), &clip);
    
    for(int posY = playarea.offset + margin; posY < game.getPlayAreaHeight(); posY += playarea.height)
    {
        SDL_Rect rect = {margin, posY, playarea.width, playarea.height};
        SDL_RenderCopy(game.getRenderer(), playarea.texture, nullptr, &rect);
    }
    
    SDL_RenderSetClipRect(game.getRenderer(), nullptr);
}

void SceneMain::renderPlayerAnimation()
{
    if(isDead)
    {
        return;
    }
    SDL_Rect src = {
        static_cast<int>(player.currentFrame * player.width / 1.5),
        static_cast<int>(static_cast<int>(player.currentAnimationType) * player.height / 1.5),
        static_cast<int>(player.width / 1.5),
        static_cast<int>(player.height /1.5)
    };
    SDL_Rect dst = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height
    };
    SDL_RenderCopy(game.getRenderer(), player.texture, &src, &dst);
}

void SceneMain::renderPlayerPoint()
{
    SDL_Rect rect = {
        static_cast<int>(playerPoint.position.x),
        static_cast<int>(playerPoint.position.y),
        playerPoint.w,
        playerPoint.h
    };
    SDL_RenderCopy(game.getRenderer(), playerPoint.texture, nullptr, &rect);
}

void SceneMain::renderPlayerBullet()
{
    for(auto& it : PlayerBullets)
    {
        SDL_Rect rect = {
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &rect);
    }
}

void SceneMain::renderPlayer()
{
    //渲染玩家动画
    renderPlayerAnimation();
    //渲染玩家判定点
    renderPlayerPoint();
}

void SceneMain::renderEnemies()
{
    //渲染敌人动画
    renderEnemiesAnimation();
}

void SceneMain::renderEnemiesAnimation()
{
    for(auto enemy : Enemies)
    {
        SDL_Rect src = getEnemyFrameRect(enemy);
        SDL_Rect dst = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopyEx(game.getRenderer(), enemy->texture, &src, &dst, 0, nullptr, enemy->isFlip);
    }
}

void SceneMain::renderEnemiesBullet()
{
    for(auto& it : EnemiesBullets)
    {
        SDL_Rect rect = {
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &rect);
    }
}

void SceneMain::renderUI()
{
    //渲染血条
    int x = margin + game.getPlayAreaWidth() + 10;
    int y = margin + 10;
    int size = 32;
    int offset = 40;
    SDL_SetTextureColorMod(uiHealth, 100, 100, 100);//颜色变淡
    for(int i = 0; i < player.maxHealth; i++)
    {
        SDL_Rect rect = {x + i * offset, y, size, size};
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &rect);
    }
    SDL_SetTextureColorMod(uiHealth, 255, 255, 255);
    for(int i = 0; i < player.currentHealth; i++)
    {
        SDL_Rect rect = {x + i * offset, y, size, size};
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &rect);
    }
    //渲染当前子弹类型
    SDL_Texture* bulletType = nullptr;
    SDL_Rect bulletRect = {x, y + offset, size, size};
    if(player.currentBulletType == PlayerBulletType::bullet1)
    {
        bulletType = BulletTextureManager["bullet1"];
    }
    else if(player.currentBulletType == PlayerBulletType::bullet2)
    {
        bulletType = BulletTextureManager["bullet2"];
    }
    else if(player.currentBulletType == PlayerBulletType::bullet3)
    {
        bulletType = BulletTextureManager["bullet3"];
    }
    SDL_RenderCopy(game.getRenderer(), bulletType, nullptr, &bulletRect);

}

bool SceneMain::ColliderEnemies(Enemy *enemy)
{
    //超界删除
    if(enemy->position.y + enemy->height > game.getPlayAreaHeight() + margin)
    {
        delete enemy;
        return true;
    }
    else//碰到玩家删除
    {
        SDL_Rect playerPointRect = {
            static_cast<int>(playerPoint.position.x),
            static_cast<int>(playerPoint.position.y),
            playerPoint.w,
            playerPoint.h
        };
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        if(SDL_HasIntersection(&playerPointRect, &enemyRect))
        {
            player.currentHealth -= 1;
            delete enemy;
            return true;
        }
    }

    //限制边界
    if(enemy->position.x < margin)
    {
        enemy->position.x = margin;
    }
    if(enemy->position.x > game.getPlayAreaWidth() + margin - enemy->width)
    {
        enemy->position.x = game.getPlayAreaWidth() + margin - enemy->width;
    }
    if(enemy->position.y < margin)
    {
        enemy->position.y = margin;
    }
    return false;
}

void SceneMain::ColliderBossBullet()
{
    BossBullets = bulletManager->getBullets();
    for(auto it = BossBullets->begin(); it != BossBullets->end(); )
    {
        EnemyBullet* bullet = *it;
        
        if( bullet->position.y > margin + game.getPlayAreaHeight() - bullet->height||
            bullet->position.y < margin ||
            bullet->position.x < margin ||
            bullet->position.x > margin + game.getPlayAreaWidth() - bullet->width)
        {
            delete bullet;
            it = BossBullets->erase(it);
        }
        else
        {
            SDL_Rect playerPointRect = {
                static_cast<int>(playerPoint.position.x),
                static_cast<int>(playerPoint.position.y),
                playerPoint.w,
                playerPoint.h
            };
            SDL_Rect bulletRect = {
                static_cast<int>(bullet->position.x),
                static_cast<int>(bullet->position.y),
                bullet->width,
                bullet->height
            };
            if(SDL_HasIntersection(&playerPointRect, &bulletRect))
            {
                playerTakeDamage(bullet->damage);
                delete bullet;
                it = BossBullets->erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::ColliderBoss()
{
    if(boss == nullptr)
    {
        return;
    }
    SDL_Rect playerPointRect = {
        static_cast<int>(playerPoint.position.x),
        static_cast<int>(playerPoint.position.y),
        playerPoint.w,
        playerPoint.h
    };
    SDL_Rect bossRect = {
        static_cast<int>(boss->getBossPos().x),
        static_cast<int>(boss->getBossPos().y),
        boss->getBossWidth(),
        boss->getBossHeight()
    };
    if(SDL_HasIntersection(&playerPointRect, &bossRect))
    {
        playerTakeDamage(1);
    }
}

void SceneMain::enemyExplode(Enemy *enemy)
{
    delete enemy;
}

void SceneMain::playerTakeDamage(int damage)
{
    if(invincible)
    {
        return;
    }

    player.currentHealth -= damage;
    invincible = true;
    invincibleTimer = 0.0f;
    
}
