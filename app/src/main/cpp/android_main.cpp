//
// Created by aosp on 2023/2/25.
//

#include "native_engine.h"

extern "C" {
void android_main(struct android_app* state);
};

void android_main(struct android_app* app) {
    auto* engine = new NativeEngine(app);
    engine->GameLoop();
    delete engine;
}
