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
                int size = opt->variants.size();
                opt->currentVariant = ((index - 1) + size) % size;
                settings->mode = (opt->currentVariant == 0);
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
                int size = opt->variants.size();
                opt->currentVariant = (index + 1) % size;
                settings->mode = (opt->currentVariant == 0);
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
    //renderButton();
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
    }
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
            int pre_w = opt.dst.w;
            int pre_x = opt.dst.x;
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
