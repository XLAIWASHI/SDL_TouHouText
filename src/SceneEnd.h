#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"
#include "Object.h"
#include <string>
#include <vector>

class SceneEnd : public Scene
{
public:
    SceneEnd();
    ~SceneEnd();

    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

    void renderBackGround();
    void renderTitle();
    void renderFinalScore();
    void renderScore(int value, int x, int y);
    void renderDigit(int digit, int x, int y);

    void loadEndfile(const std::string& filename);

private:
    Background background;
    std::vector<EndItem> endItems;
    SDL_Texture* titleTexture = nullptr;
    SDL_Texture* ui_ascii = nullptr;

    int scoreStartX = 0;
    int scoreStartY = 0;
    int scoreWidth = 0;
    int scoreHeight = 0;
    float ui_mult = 0.0f;

};

#endif