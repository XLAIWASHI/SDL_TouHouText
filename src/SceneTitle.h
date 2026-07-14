#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include "Object.h"
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
    void renderBackground();
    void renderTitle1();
    void renderTitleMenu();

    Mix_Music* bgm = nullptr;
    Background background;
    Title title;
    Title title_menu;

    //当前按钮状态
    TitleButtonType state = TitleButtonType::star;

    const float mult = 2.5f; // 按钮放大倍数
    
    const int TITLE_MENU_STAR_W = 80;
    const int TITLE_MENU_STAR_H = 32;

    const int TITLE_MENU_QUIT_W = 64;
    const int TITLE_MENU_QUIT_H = 32;
    const int TITLE_MENU_QUIT_X = 384;
    const int TITLE_MENU_QUIT_Y = 160;
    
    const int TITLE_MENU_OPTION_W = 96;
    const int TITLE_MENU_OPTION_H = 32;
    const int TITLE_MENU_OPTION_X = 400;
    const int TITLE_MENU_OPTION_Y = 416;

    const int margin = 521;

};

#endif