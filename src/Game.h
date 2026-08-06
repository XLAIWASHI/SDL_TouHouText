#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <map>

class Game
{
public:
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }
    ~Game();
    void init();
    void run();
    void clean();
    void changeScene(Scene* scene);

    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();
    SDL_Point renderTextCentered(std::string text, float posY, bool isTitle, SDL_Color color);

    //getting
    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    int getWindowWidth() { return WINDOW_W; }
    int getWindowHeight() { return WINDOW_H; }
    int getPlayAreaWidth() { return PLAY_AREA_W; }
    int getPlayAreaHeight() { return PLAY_AREA_H; }
    bool& getIsRunning() { return isRunning; }
    Settings* getSettings() { return &settings; }
    std::map<std::string, Mix_Chunk*> getSounds() { return sounds; }

    void saveSetting();
    void applySetting();
    //音乐相关
    void playBGM(const std::string& path);
    void playSound(Mix_Chunk* sound, int channel);

private:
    void loadSetting();
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Mix_Music* bgm = nullptr;
    std::string currentBgmPath = "";
    Settings settings;// 设置

    bool isRunning = true;//游戏是否运行

    //帧率相关
    int FPS = 60;
    float deltaTime;
    Uint32 frameTime;

    Scene* currentScene = nullptr;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    const int WINDOW_W = 1140;
    const int WINDOW_H = 900;
    const int PLAY_AREA_W = 600;
    const int PLAY_AREA_H = 800;
    
    //字体相关
    TTF_Font* titleFont = nullptr;
    TTF_Font* textFont = nullptr;

    //音效库
    std::map<std::string, Mix_Chunk*> sounds;
};

#endif