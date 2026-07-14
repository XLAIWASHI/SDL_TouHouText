#ifndef SCENE_OPTION_H
#define SCENE_OPTION_H

#include "Scene.h"

class SceneOption : public Scene
{
public:
    SceneOption();
    ~SceneOption();

    void init() override;
    void handleEvent(SDL_Event* event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    
};

#endif