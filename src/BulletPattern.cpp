#include "BulletPattern.h"
#include "BulletManager.h"
#include "EnemyBullet.h"

void BulletPattern::shootCircle(BulletManager &manager, SDL_FPoint position, int cnt, BulletType type)
{
    float angleStep = 360.0f / cnt;
    for(int i = 0; i < cnt; i++)
    {
        float angle = i * angleStep;
        float rad = angle * M_PI / 180.0f;
        SDL_FPoint dir;
        dir.x = cos(rad);
        dir.y = sin(rad);
        createBullet(manager, position, dir, type);
    }
}

void BulletPattern::shootFan(BulletManager &manager, SDL_FPoint position, float centerAngle, float spreadAngle, int cnt, BulletType type)
{
    if(cnt <= 0) return;
    if(cnt == 1)
    {
        float rad = centerAngle * M_PI / 180.0f;
        createBullet(manager, position, {cos(rad), sin(rad)}, type);
        return;
    }
    float startAngle = centerAngle - spreadAngle / 2;
    float step = spreadAngle / (cnt - 1); //间隔大小，5发子弹，4个间隔
    for(int i = 0; i < cnt; i++)
    {
        float angle = startAngle + i * step;
        float rad = angle * M_PI / 180.0f;
        SDL_FPoint dir;
        dir.x = cos(rad);
        dir.y = sin(rad);
        createBullet(manager, position, dir, type);
    }
}

void BulletPattern::shootSpread(BulletManager &manager, SDL_FPoint position, int cnt, int length, BulletType type)
{
    float step = length / (cnt - 1);
    int start = (position.x - length / 2);
    for(int i = 0; i < cnt; i++)
    {
        float posX = start + i * step;
        SDL_FPoint dir;
        dir.x = 0;
        dir.y = 1;
        createBullet(manager, {posX, position.y}, dir, type);
    }
}

void BulletPattern::shootAimed(BulletManager &manager, SDL_FPoint startPos, SDL_FPoint targetPos, BulletType type)
{
    float dx = targetPos.x - startPos.x;
    float dy = targetPos.y - startPos.y;
    float len = std::sqrt(dx * dx + dy * dy);
    if(len < 0.001f)
    {
        return;
    }
    dx /= len;
    dy /= len;
    createBullet(manager, startPos, {dx, dy}, type);
}

void BulletPattern::shootDirection(BulletManager &manager, SDL_FPoint position, float angle, BulletType type)
{
    float rad = angle * M_PI / 180.0f;
    SDL_FPoint dir;
    dir.x = cos(rad);
    dir.y = sin(rad);
    createBullet(manager, position, dir, type);   
}

void BulletPattern::createBullet(BulletManager &manager, SDL_FPoint position, SDL_FPoint direction, BulletType type)
{
    game.playSound(game.getSounds()["tan"], -1, 10);
    EnemyBullet* bullet = new EnemyBullet();
    bullet->direction = direction;
    int colorIdx = 1 + (rand() % 4);
    std::string key = "EnemyBullet" + std::to_string(colorIdx);
    bullet->texture = bulletTextureManager[key];
    bullet->type = type;
    SDL_QueryTexture(bullet->texture, nullptr, nullptr, &bullet->width, &bullet->height);
    bullet->width *= 1.5;
    bullet->height *= 1.5;
    bullet->position.x = position.x - bullet->width / 2;
    bullet->position.y = position.y - bullet->height / 2;
    manager.addBullet(bullet);
}
