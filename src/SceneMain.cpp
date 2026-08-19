#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneEnd.h"
#include "Game.h"
#include "EnemyBullet.h"
#include "BulletPattern.h"
#include "BulletManager.h"
#include "BossFightController.h"
#include "EffectManager.h"
#include "EffectType.h"
#include "Utils.h"
#include "Bomb.h"

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    game.playBGM("assets\\music\\bgm\\th08-06.wav");
    game.loadHighScore();
    //导入波次表
    loadWavesFromFile("data\\waves.json");
    //导入配置信息
    loadSceneData("data\\scene_main.json");
    //ui纹理
    ui_playerTexture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\UI\\front.png");
    if(ui_playerTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ui init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    ui_ascii = IMG_LoadTexture(game.getRenderer(), "assets\\image\\UI\\ascii.png");
    //item纹理
    itemsTexture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\effect\\etama2.png");
    if(itemsTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "items_texture init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
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
    //Bomb
    bombTexture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\effect\\etama4.png");
    if(bombTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "bombTexture init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
    //开启混合模式并设置判定点透明度
    SDL_SetTextureBlendMode(playerPoint.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(playerPoint.texture, 0);//透明
    //子弹资源加载
    BulletTextureManager["bullet1"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\札弹\\Sprite-0001.png");
    BulletTextureManager["bullet2"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\札弹\\札弹8.png");
    BulletTextureManager["EnemyBullet1"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹210.png");
    BulletTextureManager["EnemyBullet2"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹0.png");
    BulletTextureManager["EnemyBullet3"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹30.png");
    BulletTextureManager["EnemyBullet4"] = IMG_LoadTexture(game.getRenderer(), "assets\\image\\bullet\\粒弹\\粒弹300.png");
    
    //子弹管理
    bulletManager = new BulletManager(margin, game.getPlayAreaWidth(), game.getPlayAreaHeight());

    //特效管理
    effectManager = new EffectManager(margin);

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

    //伪3D背景初始化
    pseudo3DBg.init(game.getRenderer(), "assets\\image\\playarea\\stg5bg.png");
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
    //伪3D背景滚动
    pseudo3DBg.update(deltaTime);
    //更新玩家
    updatePlayer(deltaTime);
    //更新敌人
    updateEnemies(deltaTime);
    //更新玩家子弹
    updatePlayerBullet(deltaTime);
    if(isBomb)
    {
        updateBomb(deltaTime);
    }
    //更新敌人子弹
    updateEnemiesBullet(deltaTime);
    bulletManager->update(deltaTime);
    if(boss != nullptr)
    {
        boss->update(deltaTime);
        ColliderBossBullet();
        if(boss->getStageDefeated())
        {
            boss->setStageDefeated(false);
            game.playSound(game.getSounds()["enep00"], -1);
            addScore(30);
            for(int i = 0; i < 10; i++)
            {
                dropItem(boss->getBossPosition());
            }
        }
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
            gameOver();
            return;
        }
    }
    //生成敌人
    if(!isDead) updateWave(deltaTime);
    //更新特效
    effectManager->update(deltaTime);
    //更新items
    updateItems(deltaTime);

    if(isDead)
    {
        deadTimer += deltaTime;
        if(deadTimer >= 1.0f)
        {
            SceneEnd* sceneEnd = new SceneEnd();
            game.changeScene(sceneEnd);
        }
    }
}

void SceneMain::render()
{
    //渲染背景
    renderBackground();
    // 伪3D背景渲染
    pseudo3DBg.render(game.getRenderer());
    //渲染敌人
    renderEnemies();
    if(boss != nullptr)
    {
        boss->render(game.getRenderer());
    }
    //渲染玩家
    renderPlayer();
    //渲染玩家子弹
    renderPlayerBullet();
    //渲染敌人子弹
    renderEnemiesBullet();
    bulletManager->render(game.getRenderer());
    //渲染特效
    effectManager->render();
    if(isBomb)
    {
        renderBomb();
    }
    //渲染items
    renderItems();
    //渲染UI
    renderUI();
}

void SceneMain::clean()
{
    //清理分数字体
    if(scoreFont != nullptr)
    {
        TTF_CloseFont(scoreFont);
    }
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
    // 伪3D背景清理
    pseudo3DBg.clean();
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
    if(isDead || isDeadInterval)
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
    if(keyboardState[SDL_SCANCODE_I])
    {
        if(player.currentBomb > 0 &&
            !isBomb &&
            currentTime - player.lastBombTime > player.bombCooldown)
        {
            shootBomb();
            player.lastBombTime = currentTime;
            player.currentBomb -= 1;
        }
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

    if(isDeadInterval)
    {
        deadIntervalTime += deltaTime;
        if(deadIntervalTime >= 0.7f)
        {
            deadIntervalTime = 0.0f;
            isDeadInterval = false;

            //玩家复位
            player.position.x = margin + (game.getPlayAreaWidth() / 2 - player.width / 2);
            player.position.y = margin + game.getPlayAreaHeight() - player.height;
            player.currentBomb = player.maxBomb;

            invincible = true;
            invincibleTimer = 0.0f;
        }
    }
    if(invincible)
    {
        invincibleTimer += deltaTime;
        int blinkPhase = static_cast<int>(invincibleTimer * 10) % 2;
        SDL_SetTextureColorMod(player.texture,
            blinkPhase == 0 ? 255 : 100,
            blinkPhase == 0 ? 255 : 100,
            blinkPhase == 0 ? 255 : 100);
        if(invincibleTimer >= 1.0f)
        {
            invincibleTimer = 0.0f;
            invincible = false;
            SDL_SetTextureColorMod(player.texture, 255, 255, 255);
        }
    }
    if(player.currentHealth <= 0)
    {
        SDL_SetTextureAlphaMod(playerPoint.texture, 0);//判定点透明
        gameOver();
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

        if(bullet->type == PlayerBulletType::bullet2)
        {
            // 检查当前目标是否失效
            if(playerBulletTarget != nullptr && playerBulletTarget->health <= 0)
            {
                playerBulletTarget = nullptr;
            }

            if(playerBulletBossTarget != nullptr && boss == nullptr)
            {
                playerBulletBossTarget = nullptr;
            }

            // 没有目标，重新寻找
            if(playerBulletTarget == nullptr && playerBulletBossTarget == nullptr)
            {
                bool hasTarget = false;
                float nearestDist = 0.0f;

                // 寻找最近敌人
                for(Enemy* enemy : Enemies)
                {
                    if(enemy->health <= 0)
                        continue;

                    float cx = enemy->position.x + enemy->width / 2;
                    float cy = enemy->position.y + enemy->height / 2;
                    float dx = cx - bullet->position.x;
                    float dy = cy - bullet->position.y;
                    float dist = dx * dx + dy * dy;

                    if(!hasTarget || dist < nearestDist)
                    {
                        nearestDist = dist;
                        playerBulletTarget = enemy;
                        playerBulletBossTarget = nullptr;
                        hasTarget = true;
                    }
                }

                // 寻找Boss
                if(boss != nullptr)
                {
                    float cx = boss->getBossPos().x + boss->getBossWidth() / 2;
                    float cy = boss->getBossPos().y + boss->getBossHeight() / 2;
                    float dx = cx - bullet->position.x;
                    float dy = cy - bullet->position.y;
                    float dist = dx * dx + dy * dy;

                    if(!hasTarget || dist < nearestDist)
                    {
                        playerBulletTarget = nullptr;
                        playerBulletBossTarget = boss;
                    }
                }
            }

            SDL_FPoint targetPos = {0, 0};

            if(playerBulletTarget != nullptr)
            {
                targetPos = {
                    playerBulletTarget->position.x + playerBulletTarget->width / 2,
                    playerBulletTarget->position.y + playerBulletTarget->height / 2
                };
            }
            else if(playerBulletBossTarget != nullptr)
            {
                targetPos = {
                    playerBulletBossTarget->getBossPos().x + playerBulletBossTarget->getBossWidth() / 2,
                    playerBulletBossTarget->getBossPos().y + playerBulletBossTarget->getBossHeight() / 2
                };
            }

            // 有目标，进行追踪
            if(playerBulletTarget != nullptr || playerBulletBossTarget != nullptr)
            {
                float dx = targetPos.x - bullet->position.x;
                float dy = targetPos.y - bullet->position.y;
                float len = sqrt(dx * dx + dy * dy);

                if(len != 0)
                {
                    SDL_FPoint targetDir = {
                        dx / len,
                        dy / len
                    };

                    float smooth = 0.5f;

                    bullet->direction.x += 
                        (targetDir.x - bullet->direction.x) * smooth;

                    bullet->direction.y += 
                        (targetDir.y - bullet->direction.y) * smooth;

                    float normalLen = sqrt(
                        bullet->direction.x * bullet->direction.x +
                        bullet->direction.y * bullet->direction.y
                    );

                    bullet->direction.x /= normalLen;
                    bullet->direction.y /= normalLen;
                }
            }

            bullet->position.x += bullet->speed * bullet->direction.x * deltaTime;
            bullet->position.y += bullet->speed * bullet->direction.y * deltaTime;

            if(bullet->position.y < margin ||
               bullet->position.y > margin + game.getPlayAreaHeight() ||
               bullet->position.x < margin ||
               bullet->position.x > margin + game.getPlayAreaWidth())
            {
                delete bullet;
                it = PlayerBullets.erase(it);
                continue;
            }
        }
        else
        {
            bullet->position.y -= bullet->speed * deltaTime;

            if(bullet->position.y < margin)
            {
                delete bullet;
                it = PlayerBullets.erase(it);
                continue;
            }
        }

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
                    static_cast<int>(bullet->position.x + bullet->width * 0.25f),
                    static_cast<int>(bullet->position.y + bullet->height * 0.25f),
                    static_cast<int>(bullet->width * 0.5f),
                    static_cast<int>(bullet->height * 0.5f)
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

    game.playSound(game.getSounds()["plst"], -1, 30);

    PlayerBullet* bullet2L = new PlayerBullet();
    PlayerBullet* bullet1L = new PlayerBullet();
    PlayerBullet* bullet1R = new PlayerBullet();
    PlayerBullet* bullet2R = new PlayerBullet();

    bullet2L->texture = BulletTextureManager["bullet2"];
    bullet2L->type = PlayerBulletType::bullet2;
    bullet1L->texture = BulletTextureManager["bullet1"];
    bullet1L->type = PlayerBulletType::bullet1;
    bullet1R->texture = BulletTextureManager["bullet1"];
    bullet1R->type = PlayerBulletType::bullet1;
    bullet2R->texture = BulletTextureManager["bullet2"];
    bullet2R->type = PlayerBulletType::bullet2;

    SDL_QueryTexture(bullet1L->texture, nullptr, nullptr, &bullet1L->width, &bullet1L->height);
    bullet1L->width *= 1.5f, bullet1L->height *= 1.5f;
    SDL_QueryTexture(bullet2L->texture, nullptr, nullptr, &bullet2L->width, &bullet2L->height);
    bullet2L->width *= 1.5f, bullet2L->height *= 1.5f;

    bullet1R->width = bullet1L->width, bullet1R->height = bullet1L->height;
    bullet2R->width = bullet2L->width, bullet2R->height = bullet2L->height;

    float y = player.position.y + player.height / 2 - bullet1L->height / 2;
    float spacing = 6.0f;

    bullet2L->position.x = player.position.x - bullet2L->width - spacing;
    bullet2L->position.y = y;

    bullet1L->position.x = player.position.x;
    bullet1L->position.y = y;

    bullet1R->position.x = player.position.x + player.width - bullet1R->width;
    bullet1R->position.y = y;

    bullet2R->position.x = player.position.x + player.width + spacing;
    bullet2R->position.y = y;

    PlayerBullets.push_back(bullet2L);
    PlayerBullets.push_back(bullet1L);
    PlayerBullets.push_back(bullet1R);
    PlayerBullets.push_back(bullet2R);
}

void SceneMain::shootBomb()
{
    if(isBomb || bomb != nullptr)
    {
        return;
    }
    game.playSound(game.getSounds()["gun"], -1, 45);
    isBomb = true;
    bomb = new Bomb(player.position);
}

void SceneMain::shootEnemy(Enemy* enemy, SDL_FPoint offset)
{
    game.playSound(game.getSounds()["tan"], -1, 3);
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

void SceneMain::addScore(int value)
{
    score += value;
}

void SceneMain::addBomb(int value)
{
    player.currentBomb += value;
    if(player.currentBomb > player.maxBomb)
    {
        player.currentBomb = player.maxBomb;
    }
}

void SceneMain::addLife(int value)
{
    player.currentHealth += value;
    if(player.currentHealth > player.maxHealth)
    {
        player.currentHealth = player.maxHealth;
    }
}

void SceneMain::loadSceneData(const std::string & filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Open UIFile Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }

    json data;
    file >> data;

    //数据
    ui_offset_startX = data["digits"]["offset_startX"].get<int>();
    ui_offset_startY = data["digits"]["offset_startY"].get<int>();
    offset = data["digits"]["offset"].get<int>();
    ui_mult = data["digits"]["mult"].get<float>();
    scoreStartX = data["score"]["startX"].get<int>();
    scoreStartY = data["score"]["startY"].get<int>();
    scoreWidth = data["score"]["width"].get<int>();
    scoreHeight = data["score"]["height"].get<int>();

    //ui
    loadUI(data["uiItem"]);

    //item
    loadItem(data["item"]);

    //background 和 world
    loadBackground(data["background"], data["world"]);
}

void SceneMain::loadUI(const json& data)
{
    uiItems.clear();
    
    int index = 0;
    for(auto& item : data)
    {
        UiItem uiIt;
        std::string typeStr = item["type"].get<std::string>();
        uiIt.type = strToUi(typeStr);
        uiIt.src = {
            item["src"]["x"].get<int>(),
            item["src"]["y"].get<int>(),
            item["src"]["w"].get<int>(),
            item["src"]["h"].get<int>()
        };
        float itemMult = (uiIt.type == UiItemType::title) ? 1.8f : ui_mult;
        int dstX = (uiIt.type == UiItemType::title)
            ? margin + game.getPlayAreaWidth() + (game.getWindowWidth() - game.getPlayAreaWidth() - margin * 2 - static_cast<int>(uiIt.src.w * itemMult)) / 2
            : margin + game.getPlayAreaWidth() + ui_offset_startX;
        int dstY = (uiIt.type == UiItemType::title)
            ? margin + game.getPlayAreaHeight() - static_cast<int>(uiIt.src.h * itemMult) - 20
            : margin + ui_offset_startY + static_cast<int>(offset * index * ui_mult);
        uiIt.dst = {
            dstX,
            dstY,
            static_cast<int>(uiIt.src.w * itemMult),
            static_cast<int>(uiIt.src.h * itemMult)
        };
        if(item.contains("variants"))
        {
            for(auto& v : item["variants"])
            {
                UiItem variant;
                std::string vTypeStr = v["type"].get<std::string>();
                variant.type = strToUi(vTypeStr);
                variant.src = {
                    v["src"]["x"].get<int>(),
                    v["src"]["y"].get<int>(),
                    v["src"]["w"].get<int>(),
                    v["src"]["h"].get<int>()
                };
                variant.dst = {
                    static_cast<int>(uiIt.dst.x + uiIt.dst.w + offset),
                    static_cast<int>(uiIt.dst.y),
                    static_cast<int>(variant.src.w * ui_mult),
                    static_cast<int>(variant.src.h * ui_mult)
                };
                uiIt.variants.push_back(variant);
            }
        }
        index++;
        uiItems.push_back(uiIt);
    }
}

void SceneMain::loadItem(const json &data)
{
    for(auto& item : data)
    {
        Item itm;
        std::string typeStr = item["type"].get<std::string>();
        itm.type = strToItem(typeStr);
        SDL_Rect rect = {
            item["src"]["x"].get<int>(),
            item["src"]["y"].get<int>(),
            item["src"]["w"].get<int>(),
            item["src"]["h"].get<int>()
        };
        if(itm.type == ItemType::point)
        {
            pointSrc = rect;
        }
        else if(itm.type == ItemType::power)
        {
            powerSrc = rect;
        }
        else if(itm.type == ItemType::bomb)
        {
            bombSrc = rect;
        }
        else if(itm.type == ItemType::life)
        {
            lifeSrc = rect;
        }
    }
}

void SceneMain::loadBackground(const json& dataBackground, const json& dataWorld)
{
    //读取素材纹理区域（地板 / 墙壁）
    SDL_Rect floorSrc{0, 0, 0, 0};
    SDL_Rect wallSrc{0, 0, 0, 0};
    for (auto& item : dataBackground)
    {
        std::string type = item["type"].get<std::string>();
        SDL_Rect r = {
            item["src"]["x"].get<int>(),
            item["src"]["y"].get<int>(),
            item["src"]["w"].get<int>(),
            item["src"]["h"].get<int>()
        };
        if (type == "floor") floorSrc = r;
        else if (type == "wall") wallSrc = r;
    }

    //读取 world 参数
    const json& world = dataWorld;
    float cameraHeight = world["cameraHeight"].get<float>();
    float focal        = world["focal"].get<float>();
    float horizonY     = world["horizonY"].get<float>();
    float wallHeight   = world["wallHeight"].get<float>();
    int   zSegments    = world["zSegments"].get<int>();
    float scrollSpeed  = world["scrollSpeed"].get<float>();
    float vTiles       = world["vTiles"].get<float>();

    float halfWidth    = world["floor"]["halfWidth"].get<float>();
    float zNear        = world["floor"]["zNear"].get<float>();
    float zFar         = world["floor"]["zFar"].get<float>();

    //游玩区屏幕矩形（margin 到 margin+宽/高）
    SDL_Rect playArea = {margin, margin, game.getPlayAreaWidth(), game.getPlayAreaHeight()};

    //配置并生成网格
    pseudo3DBg.setConfig(cameraHeight, focal, horizonY, wallHeight,
                         halfWidth, zNear, zFar,
                         zSegments, scrollSpeed, vTiles,
                         floorSrc, wallSrc, playArea);
}

void SceneMain::gameOver()
{
    game.setFinalScore(score);
    game.saveHighScore(score);
    isDead = true;
}

void SceneMain::dropItem(SDL_FPoint position)
{
    Item* item = new Item();
    item->w = item->w * 2;
    item->h = item->h * 2;
    item->position.x = position.x - item->w / 2;
    item->position.y = position.y - item->h / 2;

    if(dis(gen) < 0.4f)
    {
        item->type = ItemType::point;
        item->src = pointSrc;
    }
    else if(dis(gen) < 0.7f)
    {
        item->type = ItemType::power;
        item->src = powerSrc;
    }
    else if(dis(gen) < 0.9f)
    {
        item->type = ItemType::bomb;
        item->src = bombSrc;
    }
    else if(dis(gen) < 1.0f)
    {
        item->type = ItemType::life;
        item->src = lifeSrc;
    }

    item->dst = {
        static_cast<int>(item->position.x),
        static_cast<int>(item->position.y),
        static_cast<int>(item->w),
        static_cast<int>(item->h)
    };

    item->velocity.y = -150.0f;
    item->velocity.x = dis(gen) * 20.0f - 10.0f;

    Items.push_back(item);
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
            bossFightController = new BossFightController(BulletTextureManager, *effectManager);
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

void SceneMain::updateBomb(float deltaTime)
{
    if(bomb == nullptr) return;

    bomb->timer += deltaTime;
    bomb->scale += bomb->scaleSpeed * deltaTime;

    if(bomb->scale >= bomb->maxScale)
    {
        bomb->scale = bomb->maxScale;
    }

    //更新大小
    bomb->width = bomb->baseWidth * bomb->scale;
    bomb->height = bomb->baseHeight * bomb->scale;

    //跟随玩家中心
    bomb->position.x = player.position.x + player.width / 2 - bomb->width / 2;
    bomb->position.y = player.position.y + player.height / 2 - bomb->height / 2;

    //碰撞检测
    ColliderBomb();

    

    if(bomb->timer >= bomb->lifeTime)
    {
        isBomb = false;
        delete bomb;
        bomb = nullptr;
    }
}

void SceneMain::updateItems(float deltaTime)
{
    if(Items.empty()) return;
    float gravity = 150.0f;
    for(auto it = Items.begin(); it != Items.end(); )
    {
        Item* item = *it;
        
        item->velocity.y += gravity * deltaTime;
        item->position.y += item->velocity.y * deltaTime;
        item->position.x += item->velocity.x * deltaTime;

        item->dst.x = static_cast<int>(item->position.x);
        item->dst.y = static_cast<int>(item->position.y);

        //碰撞检测
        if(ColliderItems(item))
        {
            it = Items.erase(it);
        }
        else
        {
            it++;
        }
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
            enemy->speed = 140.0f; enemy->health = 10; enemy->cooldown = 1800; break;
        case EnemyType::enemyBase2:
            enemy->speed = 100.0f; enemy->health = 5; enemy->cooldown = 1800; break;
        case EnemyType::enemyBase3:
            enemy->speed = 130.0f; enemy->health = 7; enemy->cooldown = 1800; break;
        case EnemyType::enemyBase4:
            enemy->speed = 120.0f; enemy->health = 5; enemy->cooldown = 1800; break;
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
    game.playBGM("assets\\music\\bgm\\th08-07.wav");
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
        if(it->type == PlayerBulletType::bullet2)
        {
            float angle = atan2f(it->direction.x, -it->direction.y) * 180.0f / M_PI;
            SDL_Point center = {it->width / 2, it->height / 2};
            SDL_RenderCopyEx(game.getRenderer(), it->texture, nullptr, &rect, angle, &center, SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &rect);
        }
    }
}

void SceneMain::renderPlayer()
{
    if(isDeadInterval)
    {
        return;
    }
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
    renderBossUI();

    renderPlayerUI();

}

void SceneMain::renderBossUI()
{
    if(boss != nullptr && bossFightController != nullptr && bossFightController->hasBossStage())
    {
        int stageHP = bossFightController->getStageCurrentHP();
        int stageMaxHP = bossFightController->getStageMaxHP();

        if(stageHP < 0) stageHP = 0;

        const int barWidth = game.getPlayAreaWidth() - 40;
        const int barHeight = 18;
        const int barX = margin + 20;
        const int barY = margin + 6;

        SDL_Rect bgRect = {barX, barY, barWidth, barHeight};
        SDL_SetRenderDrawColor(game.getRenderer(), 50, 50, 50, 200);
        SDL_SetRenderDrawBlendMode(game.getRenderer(), SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(game.getRenderer(), &bgRect);

        float ratio = (float)stageHP / (float)stageMaxHP;
        int fillWidth = (int)(barWidth * ratio);
        SDL_Rect fillRect = {barX, barY, fillWidth, barHeight};
        SDL_SetRenderDrawColor(game.getRenderer(), 255, 255, 255, 200);
        SDL_RenderFillRect(game.getRenderer(), &fillRect);

        SDL_SetRenderDrawColor(game.getRenderer(), 255, 255, 255, 200);
        SDL_RenderDrawRect(game.getRenderer(), &bgRect);

        SDL_SetRenderDrawBlendMode(game.getRenderer(), SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(game.getRenderer(), 0, 0, 0, 255);

        char timerText[16];
        snprintf(timerText, sizeof(timerText), "%d", stageHP);
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(game.getTextFont(), timerText, textColor);
        if(textSurface)
        {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(game.getRenderer(), textSurface);
            SDL_Rect textRect = {
                barX + barWidth - textSurface->w - 6,
                barY + (barHeight - textSurface->h) / 2,
                textSurface->w,
                textSurface->h
            };
            SDL_RenderCopy(game.getRenderer(), textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
    }
}

void SceneMain::renderPlayerUI()
{
    if(uiItems.empty())
    {
        return;
    }

    for(int i = 0; i < uiItems.size(); i++)
    {
        //血量和bomb
        if(!uiItems[i].variants.empty())
        {
            if(uiItems[i].variants[0].type == UiItemType::health)
            {
                int x = uiItems[i].variants[0].dst.x;
                int y = uiItems[i].variants[0].dst.y;
                int size = uiItems[i].variants[0].dst.w;
                SDL_SetTextureColorMod(ui_playerTexture, 100, 100, 100);
                for(int j = 0; j < player.maxHealth; j++)
                {
                    SDL_Rect rect = {x + static_cast<int>(j * offset * ui_mult), y, size, size};
                    SDL_RenderCopy(game.getRenderer(), ui_playerTexture, &uiItems[i].variants[0].src, &rect);
                }
                SDL_SetTextureColorMod(ui_playerTexture, 255, 255, 255);
                for(int j = 0; j < player.currentHealth; j++)
                {
                    SDL_Rect rect = {x + static_cast<int>(j * offset * ui_mult), y, size, size};
                    SDL_RenderCopy(game.getRenderer(), ui_playerTexture, &uiItems[i].variants[0].src, &rect);
                }
            }
            else if(uiItems[i].variants[0].type == UiItemType::bomb)
            {
                int x = uiItems[i].variants[0].dst.x;
                int y = uiItems[i].variants[0].dst.y;
                int size = uiItems[i].variants[0].dst.w;
                SDL_SetTextureColorMod(ui_playerTexture, 100, 100, 100);
                for(int j = 0; j < player.maxBomb; j++)
                {
                    SDL_Rect rect = {x + static_cast<int>(j * offset * ui_mult), y, size, size};
                    SDL_RenderCopy(game.getRenderer(), ui_playerTexture, &uiItems[i].variants[0].src, &rect);
                }
                SDL_SetTextureColorMod(ui_playerTexture, 255, 255, 255);
                for(int j = 0; j < player.currentBomb; j++)
                {
                    SDL_Rect rect = {x + static_cast<int>(j * offset * ui_mult), y, size, size};
                    SDL_RenderCopy(game.getRenderer(), ui_playerTexture, &uiItems[i].variants[0].src, &rect);
                }
            }
        }
        //渲染得分
        if(uiItems[i].type == UiItemType::score)
        {
            renderScore(score, uiItems[i].dst.x + uiItems[i].dst.w, uiItems[i].dst.y);
        }
        else if(uiItems[i].type == UiItemType::hiScore)
        {
            renderScore(game.getHighScore(), uiItems[i].dst.x + uiItems[i].dst.w, uiItems[i].dst.y);
        }
        SDL_RenderCopy(game.getRenderer(), ui_playerTexture, &uiItems[i].src, &uiItems[i].dst);
    }
}

void SceneMain::renderScore(int value, int x, int y)
{
    std::string numStr = std::to_string(value);
    
    for(char c : numStr)
    {
        int digit = c - '0';
        renderDigit(digit, x, y);
        x += static_cast<int>(scoreWidth * ui_mult);
    }
}

void SceneMain::renderDigit(int digit, int x, int y)
{
    if(digit < 0 || digit > 9) return;
    SDL_Rect src = {
        scoreStartX + scoreWidth * digit,
        scoreStartY,
        scoreWidth,
        scoreHeight
    };
    SDL_Rect dst = {x, y, static_cast<int>(scoreWidth * ui_mult), static_cast<int>(scoreHeight * ui_mult)};
    SDL_RenderCopy(game.getRenderer(), ui_ascii, &src, &dst);
}

void SceneMain::renderBomb()
{
    SDL_Rect clip = {margin, margin, game.getPlayAreaWidth(), game.getPlayAreaHeight()};
    SDL_RenderSetClipRect(game.getRenderer(), &clip);

    SDL_Rect rect = {
        static_cast<int>(bomb->position.x),
        static_cast<int>(bomb->position.y),
        bomb->width,
        bomb->height
    };

    SDL_RenderCopy(game.getRenderer(), bombTexture, nullptr, &rect);
    SDL_RenderSetClipRect(game.getRenderer(), nullptr);
}

void SceneMain::renderItems()
{
    if(Items.empty()) return;
    for(auto item : Items)
    {
        SDL_RenderCopy(game.getRenderer(), itemsTexture, &item->src, &item->dst);
    }
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
            playerTakeDamage(1);
            enemyExplode(enemy);
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
    if(isDeadInterval)
    {
        return;
    }
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
                static_cast<int>(bullet->position.x + bullet->width * 0.25f),
                static_cast<int>(bullet->position.y + bullet->height * 0.25f),
                static_cast<int>(bullet->width * 0.5f),
                static_cast<int>(bullet->height * 0.5f)
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

void SceneMain::ColliderBomb()
{
    SDL_Rect rect = {
        static_cast<int>(bomb->position.x),
        static_cast<int>(bomb->position.y),
        bomb->width,
        bomb->height
    };
    //敌人子弹
    for(auto it = EnemiesBullets.begin(); it != EnemiesBullets.end(); )
    {
        EnemyBullet* bullet = *it;
        SDL_Rect ebRect = {
            static_cast<int>(bullet->position.x + bullet->width * 0.25f),
            static_cast<int>(bullet->position.y + bullet->height * 0.25f),
            static_cast<int>(bullet->width * 0.5f),
            static_cast<int>(bullet->height * 0.5f)
        };
        if(SDL_HasIntersection(&rect, &ebRect))
        {
            delete bullet;
            it = EnemiesBullets.erase(it);
        }
        else
        {
            it++;
        }
    }
    //敌人
    for(Enemy* enemy : Enemies)
    {
        SDL_Rect ebRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        if(SDL_HasIntersection(&rect, &ebRect))
        {
            enemy->health -= 10;
        }
    }
    //Boss子弹
    if(BossBullets != nullptr)
    {
        for(auto it = BossBullets->begin(); it != BossBullets->end(); )
        {
            EnemyBullet* bullet = *it;
            SDL_Rect ebRect = {
                static_cast<int>(bullet->position.x + bullet->width * 0.25f),
                static_cast<int>(bullet->position.y + bullet->height * 0.25f),
                static_cast<int>(bullet->width * 0.5f),
                static_cast<int>(bullet->height * 0.5f)
            };
            if(SDL_HasIntersection(&rect, &ebRect))
            {
                delete bullet;
                it = BossBullets->erase(it);
            }
            else
            {
                it++;
            }
        }
    }
    //Boss
    if(boss != nullptr && !bomb->bombHitBoss)
    {
        SDL_Rect bossRect = {
            static_cast<int>(boss->getBossPos().x),
            static_cast<int>(boss->getBossPos().y),
            boss->getBossWidth(),
            boss->getBossHeight()
        };
        if(SDL_HasIntersection(&rect, &bossRect))
        {
            boss->takeDamage(150);
            bomb->bombHitBoss = true;
        }
    }
}
bool SceneMain::ColliderItems(Item *item)
{
    //超界删除
    if(item->position.y + item->h > game.getPlayAreaHeight() + margin)
    {
        delete item;
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
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->w,
            item->h
        };
        if(SDL_HasIntersection(&playerPointRect, &itemRect))
        {
            //音效
            game.playSound(game.getSounds()["item"], -1, 50);

            //item效果
            if(item->type == ItemType::point)
            {
                addScore(10);
            }
            else if(item->type == ItemType::power)
            {
                addScore(15);
            }
            else if(item->type == ItemType::bomb)
            {
                addBomb(1);
            }
            else if(item->type == ItemType::life)
            {
                addLife(1);
            }
            
            return true;
        }
    }
    //限制边界
    if(item->position.x < margin)
    {
        item->position.x = margin;
    }
    if(item->position.x > game.getPlayAreaWidth() + margin)
    {
        item->position.x = game.getPlayAreaWidth() + margin;
    }
    if(item->position.y < margin)
    {
        item->position.y = margin;
    }
    return false;
}
void SceneMain::enemyExplode(Enemy *enemy)
{
    //播放音效
    game.playSound(game.getSounds()["enep00"], -1, 30);

    SDL_FPoint position = {
        static_cast<int>(enemy->position.x) + static_cast<int>(enemy->width / 2),
        static_cast<int>(enemy->position.y) + static_cast<int>(enemy->height / 2),
    };
    effectManager->addEffect(position, EffectType::enemyDead);

    //得分
    addScore(5);

    //凋落物
    if(dis(gen) < 0.5f)
    {
        SDL_FPoint position = {
            enemy->position.x + enemy->width / 2,
            enemy->position.y + enemy->height / 2
        };
        dropItem(position);
    }

    delete enemy;
}

void SceneMain::playerTakeDamage(int damage)
{
    if(invincible || isDeadInterval)
    {
        return;
    }

    game.playSound(game.getSounds()["pldead"], -1, 45);

    //播放特效
    SDL_FPoint position = {
        static_cast<int>(player.position.x) + static_cast<int>(player.width / 2),
        static_cast<int>(player.position.y) + static_cast<int>(player.height / 2),
    };
    effectManager->addEffect(position, EffectType::playerDead);
    
    //受伤间隔
    player.currentHealth -= damage;
    isDeadInterval = true;
    deadIntervalTime = 0.0f;
}
