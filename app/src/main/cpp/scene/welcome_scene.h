//
// Created by aosp on 2023/2/25.
//

#ifndef GAME_WELCOME_SCENE_H
#define GAME_WELCOME_SCENE_H

#include "engine/engine.h"
#include "../shader/our_shader.h"
#include "../view/tex_quad.h"
#include "ui_scene.h"
#include "../utils/util.h"

/* The "welcome scene" (main menu) */
class WelcomeScene : public UiScene {
protected:
    // IDs for our buttons:
    int mPlayButtonId{};
    int mStoryButtonId{};
    int mAboutButtonId{};

    virtual void RenderBackground();
    virtual void OnButtonClicked(int id);

    void UpdateWidgetStates();

public:
    WelcomeScene();
    ~WelcomeScene();

    virtual void OnCreateWidgets();
    virtual void OnStartGraphics();
    virtual void OnKillGraphics();

    virtual void DoFrame();
};


#endif //GAME_WELCOME_SCENE_H
