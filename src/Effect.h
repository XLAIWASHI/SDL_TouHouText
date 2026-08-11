#ifndef EFFECT_H
#define EFFECT_H

#include <SDL.h>

class Effect
{
public:
    Effect();
    ~Effect() = default;

    void update(float deltaTime);

    //setting
    void setDst(SDL_FPoint position);
    void setSrc(SDL_Rect rect);

    //getting
    SDL_Rect& getEffectSrc() { return src; }
    SDL_Rect& getEffectDst() { return dst; }
    SDL_FPoint& getPosition() { return position; }

    float timer = 0.0f;
    float scale = 1.0f;
    
private:
    //SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_Rect src = {};
    SDL_Rect dst = {};

};

#endif
