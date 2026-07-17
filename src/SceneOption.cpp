#include "SceneOption.h"
#include "SceneTitle.h"
#include "Game.h"
#include "Utils.h"
#include <nlohmann/json.hpp>
#include <algorithm>

using json = nlohmann::json;

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::init()
{
    start_x = game.getWindowWidth() * 0.1f;
    start_y = game.getWindowHeight() * 0.3f;
    //加载optionitem.json
    loadOptionItemFile();
    //同步设置
    SyncSettings();
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
        // 上下键
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
                game.saveSetting();
                game.changeScene(sceneTitle);
            }
            
        }
        // 左右键
        if(event->key.keysym.scancode == SDL_SCANCODE_LEFT || event->key.keysym.scancode == SDL_SCANCODE_A)
        {
            OptionItem* opt = getCurrentOptionItem();
            Settings* settings = game.getSettings();
            if(!opt)
            {
                return;
            }
            if(state == OptionButtonType::mode)
            {
                int index = opt->currentVariant;
                int size = static_cast<int>(opt->variants.size());
                opt->currentVariant = ((index - 1) + size) % size;
                settings->mode = (opt->currentVariant == 0);
                game.applySetting();
            }
            if(state == OptionButtonType::vol)
            {
                settings->vol = std::max(0, settings->vol - 8);
                game.applySetting();
            }
            if(state == OptionButtonType::sevol)
            {
                settings->sevol = std::max(0, settings->sevol - 8);
                game.applySetting();
            }
        }
        if(event->key.keysym.scancode == SDL_SCANCODE_RIGHT || event->key.keysym.scancode == SDL_SCANCODE_D)
        {
            OptionItem* opt = getCurrentOptionItem();
            Settings* settings = game.getSettings();
            if(!opt)
            {
                return;
            }
            if(state == OptionButtonType::mode)
            {
                int index = opt->currentVariant;
                int size = static_cast<int>(opt->variants.size());
                opt->currentVariant = (index + 1) % size;
                settings->mode = (opt->currentVariant == 0);
                game.applySetting();
            }
            if(state == OptionButtonType::vol)
            {
                settings->vol = std::min(128, settings->vol + 8);
                game.applySetting();
            }
            if(state == OptionButtonType::sevol)
            {
                settings->sevol = std::min(128, settings->sevol + 8);
                game.applySetting();
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
    renderOptionsButton();
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

void SceneOption::renderOptionsButton()
{
    if(options.empty())
    {
        return;
    }
    for(int i = 0; i < options.size(); i++)
    {
        if(!options[i].variants.empty())
        {
            for(int j = 0; j < options[i].variants.size(); j++)
            {
                if(options[i].currentVariant == j)
                {
                    SDL_SetTextureColorMod(title.texture, 255, 255, 255);
                }
                else
                {
                    SDL_SetTextureColorMod(title.texture, 100, 100, 100);
                }
                SDL_RenderCopy(game.getRenderer(), title.texture, &options[i].variants[j].src, &options[i].variants[j].dst);
            }
        }
        if((OptionButtonType)state == options[i].type)
        {
            SDL_SetTextureColorMod(title.texture, 255, 255, 255);
        }
        else
        {
            SDL_SetTextureColorMod(title.texture, 100, 100, 100);
        }
        SDL_RenderCopy(game.getRenderer(), title.texture, &options[i].src, &options[i].dst);

        //音量数值
        SDL_SetTextureColorMod(title.texture, 255, 255, 255);
        if(options[i].type == OptionButtonType::vol)
        {
            int value = game.getSettings()->vol;
            int x = options[i].dst.x + static_cast<int>(optionMaxW * mult);
            int y = options[i].dst.y;
            renderNumberAsPercent(value, x, y);
        }
        if(options[i].type == OptionButtonType::sevol)
        {
            int value = game.getSettings()->sevol;
            int x = options[i].dst.x + static_cast<int>(optionMaxW * mult);
            int y = options[i].dst.y;
            renderNumberAsPercent(value, x, y);
        }
    }
}

void SceneOption::renderDigit(int digit, int x, int y)
{
    if(digit < 0 || digit > 9) return;
    SDL_Rect src = {
        digitStarX + digitW * digit,
        digitStarY,
        digitW,
        digitH
    };
    SDL_Rect dst = {x, y, static_cast<int>(digitW * mult), static_cast<int>(digitH * mult)};
    SDL_RenderCopy(game.getRenderer(), title.texture, &src, &dst);
}

void SceneOption::renderNumberAsPercent(int value, int x, int y)
{
    int percent = (value * 100) / 128;
    std::string numStr = std::to_string(percent);
    
    for(char c : numStr)
    {
        int digit = c - '0';
        renderDigit(digit, x, y);
        x += static_cast<int>(digitW * mult);
    }

    // 渲染百分号
    SDL_Rect srcPercent = {percentX, percentY, percentW, percentH};
    SDL_Rect dstPercent = {x, y, static_cast<int>(percentW * mult), static_cast<int>(percentH * mult)};
    SDL_RenderCopy(game.getRenderer(), title.texture, &srcPercent, &dstPercent);
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

    //数据
    optionMaxW = data["optionMaxW"].get<int>();

    //音量数值相关
    digitStarX = data["digits"]["startX"].get<int>();
    digitStarY = data["digits"]["startY"].get<int>();
    digitW = data["digits"]["width"].get<int>();
    digitH = data["digits"]["height"].get<int>();
    percentX = data["percent"]["percentX"].get<int>();
    percentY = data["percent"]["percentY"].get<int>();
    percentW = data["percent"]["percentW"].get<int>();
    percentH = data["percent"]["percentH"].get<int>();

    //清理旧数据
    options.clear();
    int index = 0;
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
        opt.dst = {
            start_x,
            static_cast<int>(start_y + index * spacingY * mult),
            static_cast<int>(opt.src.w * mult),
            static_cast<int>(opt.src.h * mult)
        };
        if(item.contains("variants")) //判断是否存在这个键
        {
            int pre_x = opt.dst.x;
            int pre_w = optionMaxW * mult;
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
                variant.dst = {
                    static_cast<int>(pre_x + pre_w),
                    opt.dst.y,
                    static_cast<int>(variant.src.w * mult),
                    static_cast<int>(variant.src.h * mult)
                };
                opt.variants.push_back(variant);
                pre_w = variant.dst.w;
                pre_x = variant.dst.x;
            }
        }
        options.push_back(opt);
        ++index;
    }
    state = OptionButtonType::vol;
}

void SceneOption::SyncSettings()
{
    //同步mode
    for(auto& opt : options)
    {
        if(opt.type == OptionButtonType::mode && !opt.variants.empty())
        {
            opt.currentVariant = game.getSettings()->mode ? 0 : 1;
            return;
        }
    }
}

OptionItem* SceneOption::getCurrentOptionItem()
{
    if(options.empty()) return nullptr;
    for (auto& opt : options) {
        if (opt.type == state) {
            return &opt;
        }
    }
    return nullptr;
}
