//
// Created by aosp on 2023/2/26.
//

#include "tex_quad.h"

static void put_vertex(float *v, float x, float y, float tex_u, float tex_v) {
    // position
    v[0] = x;
    v[1] = y;
    v[2] = 0.0f;

    // color
    v[3] = 1.0f;
    v[4] = 1.0f;
    v[5] = 1.0f;
    v[6] = 1.0f;

    // texture coords
    v[7] = tex_u;
    v[8] = tex_v;
}

void TexQuad::CreateGeom(float umin, float vmin, float umax, float vmax) {
    const int stride_floats =
            9;  // 3 for coords, 4 for color, 2 for tex coordinates
    const int stride_bytes = stride_floats * sizeof(GLfloat);
    int vertices = stride_floats * 4;  // 4 vertices
    auto *geom = new GLfloat[vertices];
    int geom_size = sizeof(GLfloat) * vertices;
    auto *indices = new GLushort[6];  // 6 indices
    int indices_size = sizeof(GLushort) * 6;
    float left = -mAspect * 0.5f;
    float right = mAspect * 0.5f;
    float bottom = -0.5f;
    float top = 0.5f;

    /*
      D+----------+C
       |          |
       |          |
      A+----------+B
    */

    put_vertex(geom, left, bottom, umin, vmin);                    // point A
    put_vertex(geom + stride_floats, right, bottom, umax, vmin);   // point B
    put_vertex(geom + 2 * stride_floats, right, top, umax, vmax);  // point C
    put_vertex(geom + 3 * stride_floats, left, top, umin, vmax);   // point D

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    // prepare geometry
    auto *vbuf = new VertexBuf(geom, geom_size, stride_bytes);
    vbuf->SetColorsOffset(3 * sizeof(GLfloat));
    vbuf->SetTexCoordsOffset(7 * sizeof(GLfloat));
    auto *ibuf = new IndexBuf(indices, indices_size);
    mGeom = new SimpleGeom(vbuf, ibuf);

    // clean up our temporary buffers
    delete[] geom;
    geom = nullptr;
    delete[] indices;
    indices = nullptr;
}

void TexQuad::Render(glm::mat4 *transform) {
    float aspect = SceneManager::GetInstance()->GetScreenAspect();
    glm::mat4 orthoMat = glm::ortho(0.0f, aspect, 0.0f, 1.0f);
    glm::mat4 modelMat, mat;

    bool hadDepthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);

    modelMat =
            glm::translate(glm::mat4(1.0f), glm::vec3(mCenterX, mCenterY, 0.0f));
    modelMat =
            glm::scale(modelMat, glm::vec3(mScale * mHeight, mScale * mHeight, 0.0f));
    if (transform) {
        mat = orthoMat * (*transform) * modelMat;
    } else {
        mat = orthoMat * modelMat;
    }

    mOurShader->BeginRender(mGeom->vbuf);
    mOurShader->SetTexture(mTexture);
    mOurShader->Render(mGeom->ibuf, &mat);
    mOurShader->EndRender();

    if (hadDepthTest) {
        glEnable(GL_DEPTH_TEST);
    }
}