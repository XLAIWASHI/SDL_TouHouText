#include "SceneTitle.h"
#include "SceneMain.h"
#include "Game.h"

void SceneTitle::init()
{
    // bgm = Mix_LoadMUS("assets/music/bgm/Evan Call - The Magic Within.mp3");
    bgm = Mix_LoadMUS("assets\\music\\bgm\\th08_01.mid");
    
    if(bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
        game.getIsRunning() = false;
        return;
    }
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::handleEvent(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_J)
        {
            SceneMain* sceneMain = new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}

void SceneTitle::update(float deltaTime)
{
    timer += deltaTime;
    if(timer > 1.0f)
    {
        timer -= 1.0f;
    }
}

void SceneTitle::render()
{
    //渲染标题文字
    std::string titleText = "哈哈哈哈";
    game.renderTextCentered(titleText, 0.4f, true);

    //渲染普通文字
    if(timer < 0.5f)
    {
        std::string instructions = "按J键开始游戏";
        game.renderTextCentered(instructions, 0.8f, false);
    }
}

void SceneTitle::clean()
{
    if(bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}
