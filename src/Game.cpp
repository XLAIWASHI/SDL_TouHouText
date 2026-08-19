#include "Game.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneEnd.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void Game::saveSetting()
{
    json data;
    data["vol"] = settings.vol;
    data["sevol"] = settings.sevol;
    data["mode"] = settings.mode;

    std::ofstream file("data\\setting.json");
    if(file.is_open())
    {
        file << data.dump(4);
        file.close();
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to save settings.json");
    }
}

void Game::loadHighScore()
{
    std::ifstream file("data\\hiscore.json");
    if(file.is_open())
    {
        json data;
        file >> data;
        hiScore = data.value("hiScore", 0);
    }
}

void Game::saveHighScore(int score)
{
    if(score <= hiScore) return;
    hiScore = score;
    json data;
    data["hiScore"] = hiScore;
    std::ofstream file("data\\hiscore.json");
    if(file.is_open())
    {
        file << data.dump(4);
    }
}

void Game::applySetting()
{
    //音量
    Mix_VolumeMusic(settings.vol);
    Mix_Volume(-1, settings.sevol);

    //全屏
    if(settings.mode)
    {
        SDL_RenderSetLogicalSize(renderer, WINDOW_W, WINDOW_H);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

void Game::playBGM(const std::string &path)
{
    if(currentBgmPath == path) return;

    //释放旧音乐
    if(bgm)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }

    //加载并播放新音乐
    bgm = Mix_LoadMUS(path.c_str());
    if(bgm)
    {
        Mix_PlayMusic(bgm, -1);
        currentBgmPath = path;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load BGM: %s", Mix_GetError());
        isRunning = false;
        return;
    }

}

void Game::playSound(Mix_Chunk* sound, int channel, int volume)
{
    if(sound == nullptr)
    {
        return;
    }
    Mix_VolumeChunk(sound, volume);
    Mix_PlayChannel(channel, sound, 0);
}

void Game::loadSetting()
{
    std::ifstream file("data\\setting.json");
    if(!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Open Setting Error: %s", SDL_GetError);
        isRunning = false;
        return;
    }
    
    json data;
    file >> data;

    settings.vol = data.value("vol", 100);
    settings.sevol = data.value("sevol", 100);
    settings.mode = data.value("mode", false);

    //设置音效channel数量
    Mix_AllocateChannels(32);

    // 设置音量
    Mix_VolumeMusic(settings.vol);
    Mix_Volume(-1, settings.sevol);

    //全屏
    if(settings.mode)
    {
        SDL_RenderSetLogicalSize(renderer, WINDOW_W, WINDOW_H);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

Game::Game()
{

}

Game::~Game()
{
    clean();
}

void Game::init()
{
    frameTime = 1000.0f / FPS;
    //初始化SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    //创建窗口和渲染器
    window = SDL_CreateWindow("text", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "window init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "renderer init Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    //初始化SDL_image
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image init Error: %s\n", IMG_GetError());
        isRunning = false;
    }
    
    //初始化SDL_mixer
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        isRunning = false;
    }

    //初始化SDL_ttf
    if(TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        isRunning = false;
    }

    //打开音频设备
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio! SDL_mixer Error: %s\n", Mix_GetError());
        isRunning = false;
    }

    //载入字体
    titleFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);
    if(titleFont == nullptr || textFont == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont: %s\n", TTF_GetError());
        isRunning = false;
    }

    //加载音效
    sounds["ok"] = Mix_LoadWAV("assets\\music\\sound\\se_ok00.wav");
    sounds["cancel"] = Mix_LoadWAV("assets\\music\\sound\\se_cancel00.wav");
    sounds["select"] = Mix_LoadWAV("assets\\music\\sound\\se_select00.wav");
    sounds["plst"] = Mix_LoadWAV("assets\\music\\sound\\se_plst00.wav");
    sounds["tan"] = Mix_LoadWAV("assets\\music\\sound\\se_tan00.wav");
    sounds["pldead"] = Mix_LoadWAV("assets\\music\\sound\\se_pldead00.wav");
    sounds["enep00"] = Mix_LoadWAV("assets\\music\\sound\\se_enep00.wav");
    sounds["enep01"] = Mix_LoadWAV("assets\\music\\sound\\se_enep01.wav");
    sounds["gun"] = Mix_LoadWAV("assets\\music\\sound\\se_gun00.wav");
    sounds["item"] = Mix_LoadWAV("assets\\music\\sound\\se_item00.wav");

    //载入setting.json
    loadSetting();

    currentScene = new SceneTitle();
    currentScene->init();

    //设置音效channel数量
    Mix_AllocateChannels(64);
}

void Game::run()
{
    while(isRunning)
    {
        Uint32 frameStar = SDL_GetTicks();

        SDL_Event event;
        handleEvent(&event);
        update(deltaTime);
        render();

        Uint32 frameEnd = SDL_GetTicks();
        Uint32 diff = frameEnd - frameStar;
        if(diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        }
        else
        {
            deltaTime = diff / 1000.0f;
        }

    }

}

void Game::clean()
{
    saveSetting();

    if(currentScene != nullptr)
    {
        currentScene->clean();
    }

    //清理SDL_image
    IMG_Quit();

    //清理字体
    if(titleFont != nullptr)
    {
        TTF_CloseFont(titleFont);
    }
    if(textFont != nullptr)
    {
        TTF_CloseFont(textFont);
    }

    //清理bgm
    if(bgm)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }

    //清理音效
    for(auto sound : sounds)
    {
        if(sound.second != nullptr)
        {
            Mix_FreeChunk(sound.second);
        }
    }
    sounds.clear();

    //清理SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();

    //清理SDL_ttf
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if(currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{
    //以为一帧可能会出现很多事件，我们必须一次处理完，所以要用循环
    while(SDL_PollEvent(event))
    {
        if(event->type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update(float deltaTime)
{
    currentScene->update(deltaTime);
}

void Game::render()
{
    //清空
    SDL_RenderClear(renderer);

    currentScene->render();

    //显示更新
    SDL_RenderPresent(renderer);
}

SDL_Point Game::renderTextCentered(std::string text, float posY, bool isTitle, SDL_Color color)
{
    SDL_Surface* surface;
    if(isTitle)
    {
        surface = TTF_RenderUTF8_Solid(titleFont, text.c_str(), color);
    }
    else
    {
        surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(getRenderer(), surface);
    int y = static_cast<int>((getWindowHeight() - surface->h) * posY);
    SDL_Rect rect = {
        getWindowWidth() / 2 - surface->w / 2,
        y,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(getRenderer(), texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return {rect.x + rect.w, rect.y};
}
