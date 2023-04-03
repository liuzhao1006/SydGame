//
// Created by aosp on 2023/2/26.
//

#ifndef GAME_SIMPLEGEOM_H
#define GAME_SIMPLEGEOM_H

#include "indexbuf.h"
#include "vertexbuf.h"

// Convenience class that represents a geometry in terms of a
// vertex buffer + index buffer.
class SimpleGeom {
public:
    VertexBuf *vbuf;
    IndexBuf *ibuf;

    SimpleGeom() {
        vbuf = NULL;
        ibuf = NULL;
    }

    SimpleGeom(VertexBuf *vb, IndexBuf *ib) {
        vbuf = vb;
        ibuf = ib;
    }

    SimpleGeom(VertexBuf *vb) {
        vbuf = vb;
        ibuf = NULL;
    }

    ~SimpleGeom() {
        if (vbuf) {
            delete vbuf;
        }
        if (ibuf) {
            delete ibuf;
        }
    }
};
#endif //GAME_SIMPLEGEOM_H
