//
// Created by aosp on 2023/2/26.
//

#ifndef GAME_TEXTURE_H
#define GAME_TEXTURE_H

#include "../common.hpp"

/* Represents an OpenGL texture */
class Texture {
private:
    GLuint mTextureH;

public:
    inline Texture() { mTextureH = 0; }

    // Initialize from raw RGB data. If hasAlpha is true, then it's 4 bytes per
    // pixel (RGBA), otherwise it's interpreted as 3 bytes per pixel (RGB).
    void InitFromRawRGB(int width, int height, bool hasAlpha,
                        const unsigned char *data);
    void Bind(int unit) const;
    static void Unbind();
};
#endif //GAME_TEXTURE_H
