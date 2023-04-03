//
// Created by aosp on 2023/2/26.
//

#ifndef GAME_VERTEXBUF_H
#define GAME_VERTEXBUF_H

#include "../common.hpp"

/* Represents a vertex buffer (VBO). */
class VertexBuf {
private:
    GLuint mVbo;
    GLenum mPrimitive;
    int mStride;
    int mColorsOffset;
    int mTexCoordsOffset;
    int mCount;

public:
    VertexBuf(GLfloat *geomData, int dataSize, int stride);
    ~VertexBuf();

    void BindBuffer() const;
    static void UnbindBuffer();

    int GetStride() { return mStride; }
    int GetCount() { return mCount; }
    int GetPositionsOffset() { return 0; }

    bool HasColors() { return mColorsOffset > 0; }
    int GetColorsOffset() { return mColorsOffset; }
    void SetColorsOffset(int offset) { mColorsOffset = offset; }

    bool HasTexCoords() { return mTexCoordsOffset > 0; }
    void SetTexCoordsOffset(int offset) { mTexCoordsOffset = offset; }
    int GetTexCoordsOffset() { return mTexCoordsOffset; }

    GLenum GetPrimitive() { return mPrimitive; }
    void SetPrimitive(GLenum primitive) { mPrimitive = primitive; }
};
#endif //GAME_VERTEXBUF_H
