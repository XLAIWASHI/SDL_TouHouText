#ifndef SCENE_OPTION_H
#define SCENE_OPTION_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class SceneOption : public Scene
{
public:
    SceneOption();
    ~SceneOption();

    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

private:
    Background background;
    Title title;

    void renderBackGround();
    void renderButton();

    const float mult = 2.5f; // 按钮放大倍数

    const int BTN_VOL_X = 449;
    const int BTN_VOL_Y = 161;
    const int BTN_VOL_W = 64;
    const int BTN_VOL_H = 32;

    const int BTN_SEVOL_X = 288;
    const int BTN_SEVOL_Y = 128;
    const int BTN_SEVOL_W = 96;
    const int BTN_SEVOL_H = 32;

    const int BTN_MODE_X = 384;
    const int BTN_MODE_Y = 128;
    const int BTN_MODE_W = 80;
    const int BTN_MODE_H = 32;

    const int BTN_QUIT_X = 384;
    const int BTN_QUIT_Y = 160;
    const int BTN_QUIT_W = 64;
    const int BTN_QUIT_H = 32;
};

#endif