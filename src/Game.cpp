#include "Game.h"
#include "SceneMain.h"

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

    currentScene = new SceneMain;
    currentScene->init();
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
    if(currentScene != nullptr)
    {
        currentScene->clean();
    }

    //清理SDL_image
    IMG_Quit();


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
