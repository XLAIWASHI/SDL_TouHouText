#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneOption.h"
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

    //播放音乐
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::handleEvent(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_UP || event->key.keysym.scancode == SDL_SCANCODE_W)
        {
            state = (TitleButtonType)((static_cast<int>(state) - 1 + static_cast<int>(TitleButtonType::COUNT)) % static_cast<int>(TitleButtonType::COUNT));
        }
        if(event->key.keysym.scancode == SDL_SCANCODE_DOWN || event->key.keysym.scancode == SDL_SCANCODE_S)
        {
            state = (TitleButtonType)((static_cast<int>(state) + 1) % static_cast<int>(TitleButtonType::COUNT));
        }

        // 按下触发事件
        if(event->key.keysym.scancode == SDL_SCANCODE_RETURN)
        {
            if(state == TitleButtonType::star)
            {
                SceneMain* sceneMain = new SceneMain();
                game.changeScene(sceneMain);
            }
            else if(state == TitleButtonType::option)
            {
                SceneOption* sceneOption = new SceneOption();
                game.changeScene(sceneOption);
            }
            else if(state == TitleButtonType::quit)
            {
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
    renderTitleMenu();

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

void SceneTitle::renderTitleMenu()
{
    //star
    SDL_Rect srcStar = {0, 0, TITLE_MENU_STAR_W, TITLE_MENU_STAR_H};
    SDL_Rect dstStar = {
        0,
        margin,
        static_cast<int>(TITLE_MENU_STAR_W * mult),
        static_cast<int>(TITLE_MENU_STAR_H * mult)
    };

    if(state == TitleButtonType::star)
    {
        SDL_SetTextureColorMod(title_menu.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title_menu.texture, 100, 100, 100);
    }
    SDL_RenderCopy(game.getRenderer(), title_menu.texture, &srcStar, &dstStar);

    //option
    SDL_Rect srcOption = {TITLE_MENU_OPTION_X, TITLE_MENU_OPTION_Y, TITLE_MENU_OPTION_W, TITLE_MENU_OPTION_H};
    SDL_Rect dstOption = {
        0,
        static_cast<int>(margin + TITLE_MENU_STAR_H * mult),
        static_cast<int>(TITLE_MENU_OPTION_W * mult),
        static_cast<int>(TITLE_MENU_OPTION_H * mult)
    };

    if(state == TitleButtonType::option)
    {
        SDL_SetTextureColorMod(title_menu.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title_menu.texture, 100, 100, 100);
    }
    SDL_RenderCopy(game.getRenderer(), title_menu.texture, &srcOption, &dstOption);

    //quit
    SDL_Rect srcQuit = {TITLE_MENU_QUIT_X, TITLE_MENU_QUIT_Y, TITLE_MENU_QUIT_W, TITLE_MENU_QUIT_H};
    SDL_Rect dstQuit = {
        0,
        static_cast<int>(margin + TITLE_MENU_STAR_H * mult + TITLE_MENU_OPTION_H * mult),
        static_cast<int>(TITLE_MENU_QUIT_W * mult),
        static_cast<int>(TITLE_MENU_QUIT_H * mult)
    };

    if(state == TitleButtonType::quit)
    {
        SDL_SetTextureColorMod(title_menu.texture, 255, 255, 255);
    }
    else
    {
        SDL_SetTextureColorMod(title_menu.texture, 100, 100, 100);
    }
    SDL_RenderCopy(game.getRenderer(), title_menu.texture, &srcQuit, &dstQuit);
}
