//
// Created by aosp on 2023/2/25.
//

#include "welcome_scene.h"

#include "anim.h"
#include "blurb.inl"
#include "dialog_scene.h"
#include "play_scene.h"
#include "strings.inl"

#define TITLE_POS center, 0.85f
#define TITLE_FONT_SCALE 1.0f
#define TITLE_COLOR 0.0f, 1.0f, 0.0f

// button defaults:
#define BUTTON_COLOR 0.0f, 1.0f, 0.0f
#define BUTTON_SIZE 0.2f, 0.2f
#define BUTTON_FONT_SCALE 0.5f

// button geometry
#define BUTTON_PLAY_POS center, 0.5f
#define BUTTON_PLAY_SIZE 0.4f, 0.4f
#define BUTTON_PLAY_FONT_SCALE 1.0f

// size of all side buttons (story, about)
#define BUTTON_SIDEBUTTON_WIDTH (center - 0.4f)
#define BUTTON_SIDEBUTTON_HEIGHT 0.2f
#define BUTTON_SIDEBUTTON_SIZE BUTTON_SIDEBUTTON_WIDTH, BUTTON_SIDEBUTTON_HEIGHT

// position of each side button (the buttons on the sides of the PLAY button)
#define BUTTON_STORY_POS 0.1f + 0.5f * BUTTON_SIDEBUTTON_WIDTH, 0.5f
#define BUTTON_ABOUT_POS center + 0.3f + 0.5f * BUTTON_SIDEBUTTON_WIDTH, 0.5f

WelcomeScene::WelcomeScene() = default;

WelcomeScene::~WelcomeScene() = default;

void WelcomeScene::RenderBackground() {
    RenderBackgroundAnimation(mShapeRenderer);
}

void WelcomeScene::OnButtonClicked(int id) {
    SceneManager *mgr = SceneManager::GetInstance();

    if (id == mPlayButtonId) {
        mgr->RequestNewScene(new PlayScene());
    } else if (id == mStoryButtonId) {
        mgr->RequestNewScene(
                (new DialogScene())
                        ->SetText(BLURB_STORY)
                        ->SetSingleButton(S_OK, DialogScene::ACTION_RETURN));
    } else if (id == mAboutButtonId) {
        mgr->RequestNewScene(
                (new DialogScene())
                        ->SetText(BLURB_ABOUT)
                        ->SetSingleButton(S_OK, DialogScene::ACTION_RETURN));
    }
}

void WelcomeScene::DoFrame() {
    // update widget states based on signed-in status
    UpdateWidgetStates();

    // if the sign in or cloud save process is in progress, show a wait screen.
    // Otherwise, not:
    SetWaitScreen(false);

    // draw the UI
    UiScene::DoFrame();
}

void WelcomeScene::UpdateWidgetStates() {
    // Build navigation
    AddNav(mPlayButtonId, UI_DIR_LEFT, mStoryButtonId);
    AddNav(mPlayButtonId, UI_DIR_RIGHT, mAboutButtonId);

    AddNav(mStoryButtonId, UI_DIR_RIGHT, mPlayButtonId);

    AddNav(mAboutButtonId, UI_DIR_LEFT, mPlayButtonId);
}

void WelcomeScene::OnStartGraphics() { UiScene::OnStartGraphics(); }

void WelcomeScene::OnCreateWidgets() {
    // create widgets
    float maxX = SceneManager::GetInstance()->GetScreenAspect();
    float center = 0.5f * maxX;

    // create the static title
    NewWidget()
            ->SetText(S_TITLE)
            ->SetCenter(TITLE_POS)
            ->SetTextColor(TITLE_COLOR)
            ->SetFontScale(TITLE_FONT_SCALE)
            ->SetTransition(UiWidget::TRANS_FROM_TOP);

    // create the "play" button
    mPlayButtonId = NewWidget()
            ->SetText(S_PLAY)
            ->SetTextColor(BUTTON_COLOR)
            ->SetCenter(BUTTON_PLAY_POS)
            ->SetSize(BUTTON_PLAY_SIZE)
            ->SetFontScale(BUTTON_PLAY_FONT_SCALE)
            ->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_SCALE)
            ->GetId();

    // story button
    mStoryButtonId = NewWidget()
            ->SetText(S_STORY)
            ->SetTextColor(BUTTON_COLOR)
            ->SetCenter(BUTTON_STORY_POS)
            ->SetSize(BUTTON_SIDEBUTTON_SIZE)
            ->SetFontScale(BUTTON_FONT_SCALE)
            ->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)
            ->GetId();

    // about button
    mAboutButtonId = NewWidget()
            ->SetText(S_ABOUT)
            ->SetTextColor(BUTTON_COLOR)
            ->SetCenter(BUTTON_ABOUT_POS)
            ->SetSize(BUTTON_SIDEBUTTON_SIZE)
            ->SetFontScale(BUTTON_FONT_SCALE)
            ->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)
            ->GetId();

    // "Play" button is the default button
    SetDefaultButton(mPlayButtonId);

    // enable/disable widgets as appropriate to signed in state
    UpdateWidgetStates();
}

void WelcomeScene::OnKillGraphics() { UiScene::OnKillGraphics(); }

