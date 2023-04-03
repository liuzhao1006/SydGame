//
// Created by aosp on 2023/2/26.
//

#ifndef GAME_SHAPE_RENDER_H
#define GAME_SHAPE_RENDER_H

#include "../engine/engine.h"

/* Convenience class that renders shapes (currently, only rects). The
 * coordinate system is the "normalized 2D coordinate system" -- see
 * README for more info. */
class ShapeRenderer {
private:
    TrivialShader *mTrivialShader;
    float mColor[3]{};
    SimpleGeom *mGeom;

public:
    ShapeRenderer(TrivialShader *trivialShader);
    ~ShapeRenderer();

    void SetColor(float r, float g, float b) {
        mColor[0] = r, mColor[1] = g, mColor[2] = b;
    }
    void SetColor(const float *v) {
        mColor[0] = v[0], mColor[1] = v[1], mColor[2] = v[2];
    }

    // Render a rectangle
    void RenderRect(float centerX, float centerY, float width, float height);
};

#endif //GAME_SHAPE_RENDER_H
