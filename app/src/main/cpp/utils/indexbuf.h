//
// Created by aosp on 2023/2/25.
//

#ifndef GAME_INDEXBUF_H
#define GAME_INDEXBUF_H

#include "../common.hpp"

/* Represents an index buffer (IBO). */
class IndexBuf {
public:
    IndexBuf(GLushort *data, int dataSizeBytes);
    ~IndexBuf();

    void BindBuffer() const;
    static void UnbindBuffer();
    int GetCount() { return mCount; }

private:
    GLuint mIbo{};
    int mCount;
};
#endif //GAME_INDEXBUF_H
