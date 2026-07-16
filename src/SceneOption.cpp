#include "SceneOption.h"
#include "SceneTitle.h"
#include "Game.h"
#include "Utils.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::init()
{
    //加载optionitem.json
    loadOptionItemFile();
    //加载背景图片
    background.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\title\\star.png");
    if(background.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }

    //加载按钮图片
    title.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\title\\title.png");
    if(title.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }
}

void SceneOption::handleEvent(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_UP || event->key.keysym.scancode == SDL_SCANCODE_W)
        {
            state = (OptionButtonType)((static_cast<int>(state) - 1 + static_cast<int>(OptionButtonType::COUNT)) % static_cast<int>(OptionButtonType::COUNT));
        }
        if(event->key.keysym.scancode == SDL_SCANCODE_DOWN || event->key.keysym.scancode == SDL_SCANCODE_S)
        {
            state = (OptionButtonType)((static_cast<int>(state) + 1) % static_cast<int>(OptionButtonType::COUNT));
        }

        // 按下触发事件
        if(event->key.keysym.scancode == SDL_SCANCODE_RETURN)
        {
            if(state == OptionButtonType::quit)
            {
                SceneTitle* sceneTitle = new SceneTitle();
                game.changeScene(sceneTitle);
            }
            if(state == OptionButtonType::mode)
            {

            }
        }
    }
}

void SceneOption::update(float deltaTime)
{
}

void SceneOption::render()
{
    //渲染背景
    renderBackGround();
    //渲染按钮
    renderButton();
}

void SceneOption::clean()
{
    if(background.texture != nullptr)
    {
        SDL_DestroyTexture(background.texture);
    }

    if(title.texture != nullptr)
    {
        SDL_DestroyTexture(title.texture);
    }
}

void SceneOption::renderBackGround()
{
    SDL_RenderCopy(game.getRenderer(), background.texture, nullptr, nullptr);
}

void SceneOption::renderButton()
{
    //vol
    int vol_x = game.getWindowWidth() * 0.1f;
    int vol_y = game.getWindowHeight() * 0.3f;
    SDL_Rect srcVolRect = {BTN_VOL_X, BTN_VOL_Y, BTN_VOL_W, BTN_VOL_H};
    SDL_Rect dstVolRect = {
        vol_x,
        vol_y,
        static_cast<int>(BTN_VOL_W * mult),
        static_cast<int>(BTN_VOL_H * mult)
    };

    if(state == OptionButtonType::vol)
    {
        SDL_SetTextureColorMod(title.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title.texture, 100, 100, 100);
    }

    SDL_RenderCopy(game.getRenderer(), title.texture, &srcVolRect, &dstVolRect);
    //此处留空

    //sevol
    int sevol_x = vol_x;
    int sevol_y = vol_y + BTN_VOL_H * mult;
    SDL_Rect srcSEVolRect = {BTN_SEVOL_X, BTN_SEVOL_Y, BTN_SEVOL_W, BTN_SEVOL_H};
    SDL_Rect dstSEVolRect = {
        sevol_x,
        sevol_y,
        static_cast<int>(BTN_SEVOL_W * mult),
        static_cast<int>(BTN_SEVOL_H * mult)
    };

    if(state == OptionButtonType::sevol)
    {
        SDL_SetTextureColorMod(title.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title.texture, 100, 100, 100);
    }

    SDL_RenderCopy(game.getRenderer(), title.texture, &srcSEVolRect, &dstSEVolRect);
    //此处留空

    //mode
    int mode_x = vol_x;
    int mode_y = sevol_y + BTN_SEVOL_H * mult;
    SDL_Rect srcModeRect = {BTN_MODE_X, BTN_MODE_Y, BTN_MODE_W, BTN_MODE_H};
    SDL_Rect dstModeRect = {
        mode_x,
        mode_y,
        static_cast<int>(BTN_MODE_W * mult),
        static_cast<int>(BTN_MODE_H * mult)
    };

    if(state == OptionButtonType::mode)
    {
        SDL_SetTextureColorMod(title.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title.texture, 100, 100, 100);
    }

    SDL_RenderCopy(game.getRenderer(), title.texture, &srcModeRect, &dstModeRect);

    //fullsceen
    int mode_fullsceen_x = vol_x + BTN_MODE_W * mult;
    int mode_fullsceen_y = mode_y;
    SDL_Rect srcMode_fullScreenRect = {BTN_MODE_FULLSCEEN_X, BTN_MODE_FULLSCEEN_Y, BTN_MODE_FULLSCEEN_W, BTN_MODE_FULLSCEEN_H};
    SDL_Rect dstMode_fullScreenRect = {
        mode_fullsceen_x,
        mode_fullsceen_y,
        static_cast<int>(BTN_MODE_FULLSCEEN_W * mult),
        static_cast<int>(BTN_MODE_FULLSCEEN_H * mult)
    };
    SDL_RenderCopy(game.getRenderer(), title.texture, &srcMode_fullScreenRect, &dstMode_fullScreenRect);
    
    //window
    int mode_window_x = mode_fullsceen_x + BTN_MODE_FULLSCEEN_W * mult;
    int mode_window_y = mode_y;
    SDL_Rect srcMode_windowRect = {BTN_MODE_WINDOW_X, BTN_MODE_WINDOW_Y, BTN_MODE_WINDOW_W, BTN_MODE_WINDOW_H};
    SDL_Rect dstMode_windowRect = {
        mode_window_x,
        mode_window_y,
        static_cast<int>(BTN_MODE_WINDOW_W * mult),
        static_cast<int>(BTN_MODE_WINDOW_H * mult)
    };
    SDL_RenderCopy(game.getRenderer(), title.texture, &srcMode_windowRect, &dstMode_windowRect);
    //此处留空

    //quit
    int quit_x = vol_x;
    int quit_y = mode_y + BTN_MODE_H * mult;
    SDL_Rect srcQuitRect = {BTN_QUIT_X, BTN_QUIT_Y, BTN_QUIT_W, BTN_QUIT_H};
    SDL_Rect dstQuitRect = {
        quit_x,
        quit_y,
        static_cast<int>(BTN_QUIT_W * mult),
        static_cast<int>(BTN_QUIT_H * mult)
    };

    if(state == OptionButtonType::quit)
    {
        SDL_SetTextureColorMod(title.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title.texture, 100, 100, 100);
    }

    SDL_RenderCopy(game.getRenderer(), title.texture, &srcQuitRect, &dstQuitRect);
    //此处留空
}

void SceneOption::loadOptionItemFile()
{
    std::ifstream file("data\\optionItem.json");
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Open OptionItem Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }

    json data;
    file >> data;

    //清理旧数据
    options.clear();

    for(auto& item : data["option"])
    {
        OptionItem opt;
        std::string typeStr = item["type"].get<std::string>();
        opt.type = strToOption(typeStr);
        opt.src = {
            item["src"]["x"].get<int>(),
            item["src"]["y"].get<int>(),
            item["src"]["w"].get<int>(),
            item["src"]["h"].get<int>()
        };
        if(item.contains("variants")) //判断是否存在这个键
        {
            for(auto& v : item["variants"])
            {
                OptionItem variant;
                std::string vTypeStr = v["type"].get<std::string>();
                variant.type = strToOption(vTypeStr);
                variant.src = {
                    v["src"]["x"].get<int>(),
                    v["src"]["y"].get<int>(),
                    v["src"]["w"].get<int>(),
                    v["src"]["h"].get<int>()
                };
                opt.variants.push_back(variant);
            }
        }
        options.push_back(opt);
    }
    state = OptionButtonType::vol;
}

