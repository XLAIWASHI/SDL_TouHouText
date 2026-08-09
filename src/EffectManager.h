#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "EffectType.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list>

class Effect;

class EffectManager
{
public:
    EffectManager(int margin);
    ~EffectManager();

    void update(float deltaTime);
    void render();
    void clean();

    void addEffect(SDL_FPoint position, EffectType type);
    void loadEffectFile();

private:
    SDL_Texture* effectTextures = nullptr;
    std::list<Effect*> effects;
    Game& game;

    SDL_Rect effect_enemyDeadSrcRect = {};
    SDL_Rect effect_playerDeadSrcRect = {};

    int margin = 0;
    float lifeTime = 0.0f;
    float maxScale = 0.0f;
    float scaleSpeed = 0.0f;
};

#endif