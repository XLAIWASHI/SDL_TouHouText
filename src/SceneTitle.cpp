#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
#include "Game.h"
#include "Utils.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void SceneTitle::init()
{
    //加载optionitem.json
    loadTitleItemFile();
    // bgm = Mix_LoadMUS("assets/music/bgm/Evan Call - The Magic Within.mp3");
    game.playBGM("assets\\music\\bgm\\th08-01.wav");

    //加载背景图片
    background.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\title\\star.png");
    if(background.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load title_background: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(background.texture, nullptr, nullptr, &background.width, &background.height);

    //加载标题图片
    title.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\title\\starTitle1.png");
    if(title.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load title: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(title.texture, nullptr, nullptr, &title.width, &title.height);
    title.width *= 1.5;
    title.height *= 1.5;

    //加载标题选项
    title_menu.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\title\\title.png");
    if(title_menu.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load title_menu: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(title_menu.texture, nullptr, nullptr, &title_menu.width, &title_menu.height);

}

void SceneTitle::handleEvent(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_UP || event->key.keysym.scancode == SDL_SCANCODE_W)
        {
            game.playSound(game.getSounds()["select"], -1);
            state = (TitleButtonType)((static_cast<int>(state) - 1 + static_cast<int>(TitleButtonType::COUNT)) % static_cast<int>(TitleButtonType::COUNT));
        }
        if(event->key.keysym.scancode == SDL_SCANCODE_DOWN || event->key.keysym.scancode == SDL_SCANCODE_S)
        {
            game.playSound(game.getSounds()["select"], -1);
            state = (TitleButtonType)((static_cast<int>(state) + 1) % static_cast<int>(TitleButtonType::COUNT));
        }

        // 按下触发事件
        if(event->key.keysym.scancode == SDL_SCANCODE_RETURN)
        {
            if(state == TitleButtonType::start)
            {
                game.playSound(game.getSounds()["ok"], -1);
                SceneMain* sceneMain = new SceneMain();
                game.changeScene(sceneMain);
            }
            else if(state == TitleButtonType::option)
            {
                game.playSound(game.getSounds()["ok"], -1);
                SceneOption* sceneOption = new SceneOption();
                game.changeScene(sceneOption);
            }
            else if(state == TitleButtonType::quit)
            {
                game.playSound(game.getSounds()["cancel"], -1);
                SDL_Delay(200);
                game.getIsRunning() = false;
            }
        }
    }
}

void SceneTitle::update(float deltaTime)
{
    
}

void SceneTitle::render()
{
    //渲染背景图片
    renderBackground();
    //渲染标题
    renderTitle1();
    //渲染按钮
    renderTitleButton();

}

void SceneTitle::clean()
{
    if(background.texture != nullptr)
    {
        SDL_DestroyTexture(background.texture);
    }

    if(title.texture != nullptr)
    {
        SDL_DestroyTexture(title.texture);
    }

    if(title_menu.texture != nullptr)
    {
        SDL_DestroyTexture(title_menu.texture);
    }

}

void SceneTitle::renderBackground()
{
    SDL_RenderCopy(game.getRenderer(), background.texture, nullptr, nullptr);
}

void SceneTitle::renderTitle1()
{
    SDL_Rect rect = {
        static_cast<int>(game.getWindowWidth() * 0.6f),
        static_cast<int>(game.getWindowHeight() * 0.1f),
        title.width,
        title.height
    };
    SDL_RenderCopy(game.getRenderer(), title.texture, nullptr, &rect);
}

void SceneTitle::renderTitleButton()
{
    if(titles.empty())
    {
        return;
    }
    for(int i = 0; i < titles.size(); i++)
    {
        if((TitleButtonType)state == titles[i].type)
        {
            SDL_SetTextureColorMod(title_menu.texture, 255, 255, 255);
        }
        else
        {
            SDL_SetTextureColorMod(title_menu.texture, 100, 100, 100);
        }
        SDL_RenderCopy(game.getRenderer(), title_menu.texture, &titles[i].src, &titles[i].dst);
    }
}

void SceneTitle::loadTitleItemFile()
{
    std::ifstream file("data\\optionItem.json");
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Open TitleItem Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }

    json data;
    file >> data;

    margin = data["titleMargin"].get<int>();

    //清理旧数据
    titles.clear();
    int index = 0;
    for(auto& item : data["title"])
    {
        TitleItem tit;
        std::string typeStr = item["type"].get<std::string>();
        tit.type = strToTitle(typeStr);
        tit.src = {
            item["src"]["x"].get<int>(),
            item["src"]["y"].get<int>(),
            item["src"]["w"].get<int>(),
            item["src"]["h"].get<int>()
        };
        tit.dst = {
            0,
            static_cast<int>(margin + index * tit.src.h * mult),
            static_cast<int>(tit.src.w * mult),
            static_cast<int>(tit.src.h * mult)
        };
        titles.push_back(tit);
        ++index;
    }
    state = TitleButtonType::start;
}
