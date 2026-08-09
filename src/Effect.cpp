#include "Effect.h"

Effect::Effect()
{

}

void Effect::update(float deltaTime)
{
    
}

void Effect::setDst(SDL_FPoint position)
{
    this->position = position;
    dst = {
        static_cast<int>(this->position.x) - static_cast<int>(src.w / 2),
        static_cast<int>(this->position.y) - static_cast<int>(src.h / 2),
        src.w,
        src.h
    };

}

void Effect::setSrc(SDL_Rect rect)
{
    src = rect;
}
