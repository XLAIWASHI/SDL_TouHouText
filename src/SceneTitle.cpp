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
    //渲染背景图片
    renderBackground();
    //绘制标题
    renderTitle1();

    //渲染标题文字
    SDL_Color color = {0, 0, 0, 0};

    //渲染普通文字
    if(timer < 0.5f)
    {
        std::string instructions = "按J键开始游戏";
        game.renderTextCentered(instructions, 0.8f, false, color);
    }
}

void SceneTitle::clean()
{
    if(bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }

    if(background.texture != nullptr)
    {
        SDL_DestroyTexture(background.texture);
    }

    if(title.texture != nullptr)
    {
        SDL_DestroyTexture(title.texture);
    }
}

void SceneTitle::renderBackground()
{
    SDL_RenderCopy(game.getRenderer(), background.texture, nullptr, nullptr);
}

void SceneTitle::renderTitle1()
{
    SDL_Rect rect = {game.getWindowWidth() * 0.6, 0, title.width, title.height};
    SDL_RenderCopy(game.getRenderer(), title.texture, nullptr, &rect);
}
