#include "SceneOption.h"
#include "Game.h"

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::init()
{
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
    SDL_RenderCopy(game.getRenderer(), title.texture, &srcModeRect, &dstModeRect);
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
    SDL_RenderCopy(game.getRenderer(), title.texture, &srcQuitRect, &dstQuitRect);
    //此处留空
}
