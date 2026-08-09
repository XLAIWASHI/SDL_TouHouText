#include "EffectManager.h"
#include "Effect.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

EffectManager::EffectManager(int margin) : game(Game::getInstance()), margin(margin)
{
    loadEffectFile();
    effectTextures = IMG_LoadTexture(game.getRenderer(), "assets\\image\\effect\\etama2.png");
    if(effectTextures == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "effectTexture init Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }
}

EffectManager::~EffectManager()
{
}

void EffectManager::update(float deltaTime)
{
    for(auto it = effects.begin(); it != effects.end(); )
    {
        Effect* effect = *it;

        effect->timer += deltaTime;
        effect->scale += scaleSpeed * deltaTime;

        if(effect->scale >= maxScale)
        {
            effect->scale = maxScale;
        }

        //更新大小
        effect->getEffectDst().w = effect->getEffectSrc().w * effect->scale;
        effect->getEffectDst().h = effect->getEffectSrc().h * effect->scale;

        //保持中心
        effect->getEffectDst().x = effect->getPosition().x - effect->getEffectDst().w / 2;
        effect->getEffectDst().y = effect->getPosition().y - effect->getEffectDst().h / 2;
        
        if(effect->timer >= lifeTime)
        {
            delete effect;
            it = effects.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void EffectManager::render()
{
    SDL_Rect clip = {margin, margin, game.getPlayAreaWidth(), game.getPlayAreaHeight()};
    SDL_RenderSetClipRect(game.getRenderer(), &clip);
    
    for(auto& effect : effects)
    {
        SDL_RenderCopy(game.getRenderer(), effectTextures, &effect->getEffectSrc(), &effect->getEffectDst());
    }

    SDL_RenderSetClipRect(game.getRenderer(), nullptr);
}

void EffectManager::clean()
{
    if(effectTextures != nullptr)
    {
        SDL_DestroyTexture(effectTextures);
    }


    for(auto& effect : effects)
    {
        if(effect != nullptr)
        {
            delete effect;
        }
    }
    effects.clear();
}

void EffectManager::addEffect(SDL_FPoint position, EffectType type)
{
    Effect* effect = new Effect();
    if(type == EffectType::enemyDead)
    {
        effect->setSrc(effect_enemyDeadSrcRect);
    }
    else if(type == EffectType::playerDead)
    {
        effect->setSrc(effect_playerDeadSrcRect);
    }
    effect->setDst(position);
    effects.push_back(effect);
}

void EffectManager::loadEffectFile()
{
    std::ifstream file("data\\effect.json");
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Open EffectFile Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }

    json data;
    file >> data;

    effect_enemyDeadSrcRect = {
        data["effect"][0]["src"]["x"].get<int>(),
        data["effect"][0]["src"]["y"].get<int>(),
        data["effect"][0]["src"]["w"].get<int>(),
        data["effect"][0]["src"]["h"].get<int>()
    };

    effect_playerDeadSrcRect = {
        data["effect"][1]["src"]["x"].get<int>(),
        data["effect"][1]["src"]["y"].get<int>(),
        data["effect"][1]["src"]["w"].get<int>(),
        data["effect"][1]["src"]["h"].get<int>()
    };

    lifeTime = data["scaleSetting"]["lifeTime"];
    maxScale = data["scaleSetting"]["maxScale"];
    scaleSpeed = data["scaleSetting"]["scaleSpeed"];

}
