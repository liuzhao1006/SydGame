//
// Created by aosp on 2023/2/25.
//
#ifndef syd_native_engine_hpp
#define syd_native_engine_hpp

#include "common.hpp"

struct NativeEngineSavedState {
    bool mHasFocus;
};

class NativeEngine {
public:
    NativeEngine(struct android_app *app);

    ~NativeEngine();

    void GameLoop();

    JNIEnv *GetJniEnv();

    android_app *GetAndroidApp();

    static NativeEngine *GetInstance();

private:
    bool mHasFocus, mIsVisible, mHasWindow;
    bool mHasGLObjects;

    int mApiVersion;

    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLContext mEglContext;
    EGLConfig mEglConfig;

    int mSurfWidth, mSurfHeight;

    struct android_app *mApp;

    struct NativeEngineSavedState mState;

    JNIEnv *mJniEnv;

    bool mIsFirstFrame;

    bool InitDisplay();

    void KillDisplay();

    bool InitSurface();

    void KillSurface();

    bool InitContext();

    void KillContext();

    bool HandleEglError(EGLint error);

    bool InitGLObjects();

    void KillGLObjects();

    void ConfigureOpenGL();

    bool PrepareToRender();

    void DoFrame();

    bool IsAnimating() const;

public:
    void HandleCommand(int32_t cmd);

    bool HandleInput(AInputEvent *event);

};

#endif //syd_native_engine_hpp