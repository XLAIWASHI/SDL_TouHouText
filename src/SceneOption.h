#ifndef SCENE_OPTION_H
#define SCENE_OPTION_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <fstream>
#include <vector>
#include <string>

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
    void renderBackGround();
    void renderOptionsButton();
    void renderDigit(int digit, int x, int y);// 渲染一个数字
    void renderNumberAsPercent(int value, int x, int y);// 渲染数值和百分号

    void loadOptionItemFile();
    void SyncSettings();// 同步设置
    //getting
    OptionItem* getCurrentOptionItem();

    Background background;
    Title title;

    std::vector<OptionItem> options;
    OptionButtonType state = OptionButtonType::vol;
    
    int start_x = 0;
    int start_y = 0;
    int optionMaxW = 0;
    const int spacingY = 32;// Y间距
    const float mult = 2.5f; // 按钮放大倍数

    //渲染音量数值相关
    int digitW = 0;
    int digitH = 0;
    int digitStarX = 0;
    int digitStarY = 0;
    int percentX = 0;
    int percentY = 0;
    int percentW = 0;
    int percentH = 0;
};

#endif