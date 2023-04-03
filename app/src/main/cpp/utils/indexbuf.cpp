//
// Created by aosp on 2023/2/25.
//

#include "indexbuf.h"

IndexBuf::IndexBuf(GLushort *data, int dataSizeBytes) {
    mCount = dataSizeBytes / sizeof(GLushort);

    glGenBuffers(1, &mIbo);
    BindBuffer();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSizeBytes, data, GL_STATIC_DRAW);
    UnbindBuffer();
}

IndexBuf::~IndexBuf() {
    glDeleteBuffers(1, &mIbo);
    mIbo = 0;
}

void IndexBuf::BindBuffer() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo); }

void IndexBuf::UnbindBuffer() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
