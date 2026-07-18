#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>

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
    void renderTitleButton();

    void loadTitleItemFile();

    Background background;
    Title title;
    Title title_menu;
    std::vector<TitleItem> titles;

    //当前按钮状态
    TitleButtonType state = TitleButtonType::start;

    const float mult = 2.5f; // 按钮放大倍数

    int margin = 0;

};

#endif