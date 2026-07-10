#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class SceneTitle : public Scene
{
public:
    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

private:
    Mix_Music* bgm = nullptr;
    float timer = 0.0f;
};

#endif