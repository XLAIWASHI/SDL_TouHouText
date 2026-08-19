#include "SceneEnd.h"
#include "Game.h"
#include "Utils.h"
#include "SceneTitle.h"
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SceneEnd::SceneEnd()
{
}

SceneEnd::~SceneEnd()
{
}

void SceneEnd::init()
{
    loadEndfile("data\\end.json");

    background.texture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\end\\result.jpg");
    if(background.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load end_background: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }
    SDL_QueryTexture(background.texture, nullptr, nullptr, &background.width, &background.height);

    titleTexture = IMG_LoadTexture(game.getRenderer(), "assets\\image\\end\\result03.png");
    if(titleTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load end_title: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }

    ui_ascii = IMG_LoadTexture(game.getRenderer(), "assets\\image\\UI\\ascii.png");
    if(ui_ascii == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load ui_title: %s", IMG_GetError());
        game.getIsRunning() = false;
        return;
    }
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_RETURN ||
           event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            SceneTitle* sceneTitle = new SceneTitle();
            game.changeScene(sceneTitle);
        }
    }
}

void SceneEnd::update(float deltaTime)
{
}

void SceneEnd::render()
{
    renderBackGround();

    renderTitle();

    renderFinalScore();
    
}

void SceneEnd::clean()
{
    if(background.texture != nullptr)
    {
        SDL_DestroyTexture(background.texture);
    }
    if(titleTexture != nullptr)
    {
        SDL_DestroyTexture(titleTexture);
    }
    if(ui_ascii != nullptr)
    {
        SDL_DestroyTexture(ui_ascii);
    }
}

void SceneEnd::renderBackGround()
{
    SDL_RenderCopy(game.getRenderer(), background.texture, nullptr, nullptr);
}

void SceneEnd::renderTitle()
{
    for(auto& item : endItems)
    {
        SDL_RenderCopy(game.getRenderer(), titleTexture, &item.src, &item.dst);
    }
}

void SceneEnd::renderFinalScore()
{
    if(endItems.empty()) return;
    EndItem& title = endItems[0];
    int finalScore = game.getFinalScore();
    std::string numStr = std::to_string(finalScore);
    int digitW = static_cast<int>(scoreWidth * ui_mult);
    int totalW = static_cast<int>(numStr.size()) * digitW;
    int x = title.dst.x + title.dst.w / 2 - totalW / 2;
    int y = title.dst.y + title.dst.h + 60;
    renderScore(finalScore, x, y);
}

void SceneEnd::renderScore(int value, int x, int y)
{
    std::string numStr = std::to_string(value);
    
    for(char c : numStr)
    {
        int digit = c - '0';
        renderDigit(digit, x, y);
        x += static_cast<int>(scoreWidth * ui_mult);
    }
}

void SceneEnd::renderDigit(int digit, int x, int y)
{
    if(digit < 0 || digit > 9) return;
    SDL_Rect src = {
        scoreStartX + scoreWidth * digit,
        scoreStartY,
        scoreWidth,
        scoreHeight
    };
    SDL_Rect dst = {x, y, static_cast<int>(scoreWidth * ui_mult), static_cast<int>(scoreHeight * ui_mult)};
    SDL_RenderCopy(game.getRenderer(), ui_ascii, &src, &dst);
}

void SceneEnd::loadEndfile(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Open TitleItem Error: %s\n", SDL_GetError());
        game.getIsRunning() = false;
        return;
    }

    json data;
    file >> data;

    scoreStartX = data["score"]["startX"].get<int>();
    scoreStartY = data["score"]["startY"].get<int>();
    scoreWidth = data["score"]["width"].get<int>();
    scoreHeight = data["score"]["height"].get<int>();
    ui_mult = data["digits"]["mult"].get<float>();

    endItems.clear();
    for(auto& item : data["end"])
    {
        EndItem eit;
        std::string typeStr = item["type"].get<std::string>();
        eit.type = strToEnd(typeStr);
        eit.src = {
            item["src"]["x"].get<int>(),
            item["src"]["y"].get<int>(),
            item["src"]["w"].get<int>(),
            item["src"]["h"].get<int>()
        };

        int w = static_cast<int>(eit.src.w * 3);
        int h = static_cast<int>(eit.src.h * 3);
        eit.dst = {
            static_cast<int>(game.getWindowWidth() / 2) - static_cast<int>(w / 2),
            static_cast<int>(game.getWindowHeight() * 0.10f),
            w,
            h
        };

        endItems.push_back(eit);
    }
}