//
// Created by aosp on 2023/2/26.
//

#include "our_shader.h"
#include "../data/our_shader.inl"

OurShader::OurShader() : Shader() {
    mColorLoc = (GLint)-1;
    mTintLoc = -1;
    mTexCoordLoc = (GLint)-1;
    mSamplerLoc = -1;
    mPointLightPosLoc = -1;
    mPointLightColorLoc = -1;
}

OurShader::~OurShader() = default;

void OurShader::Compile() {
    // let base class handle compilation
    Shader::Compile();

    // get location of the color attribute
    BindShader();
    mColorLoc = glGetAttribLocation(mProgramH, "a_Color");
    if (mColorLoc < 0) {
        LOGE("*** Couldn't get color attrib location from shader (OurShader).");
        ABORT_GAME;
    }
    mTintLoc = glGetUniformLocation(mProgramH, "u_Tint");
    if (mTintLoc < 0) {
        LOGE("*** Couldn't get Tint attrib location from shader (OurShader).");
        ABORT_GAME;
    }
    mSamplerLoc = glGetUniformLocation(mProgramH, "u_Sampler");
    if (mSamplerLoc < 0) {
        LOGE("*** Couldn't get sampler location from shader (OurShader).");
        ABORT_GAME;
    }
    mTexCoordLoc = (GLuint)glGetAttribLocation(mProgramH, "a_TexCoord");
    if (mTexCoordLoc < 0) {
        LOGE(
                "*** Couldn't get tex coord attribute location from shader "
                "(OurShader).");
        ABORT_GAME;
    }
    mPointLightPosLoc = glGetUniformLocation(mProgramH, "u_PointLightPos");
    if (mPointLightPosLoc < 0) {
        LOGE("**** Couldn't get point light position uniform (OurShader).");
        ABORT_GAME;
    }
    mPointLightColorLoc = glGetUniformLocation(mProgramH, "u_PointLightColor");
    if (mPointLightColorLoc < 0) {
        LOGE("**** Couldn't get point light position uniform (OurShader).");
        ABORT_GAME;
    }
    UnbindShader();
}

void OurShader::SetTintColor(float r, float g, float b) {
    MY_ASSERT(mTintLoc >= 0);
    MY_ASSERT(mPreparedVertexBuf != nullptr);
    glUniform4f(mTintLoc, r, g, b, 1.0f);
}

void OurShader::SetTexture(Texture* t) {
    MY_ASSERT(mPreparedVertexBuf != nullptr);
    t->Bind(GL_TEXTURE0);
    glUniform1i(mSamplerLoc, 0);
}

void OurShader::EnablePointLight(glm::vec3 pos, float r, float g, float b) {
    MY_ASSERT(mPreparedVertexBuf != nullptr);
    glUniform4f(mPointLightColorLoc, r, g, b, 1.0);
    glUniform4f(mPointLightPosLoc, pos.x, pos.y, pos.z, 1.0);
}

void OurShader::DisablePointLight() {
    MY_ASSERT(mPreparedVertexBuf != nullptr);
    glUniform4f(mPointLightColorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
}

void OurShader::BeginRender(VertexBuf* geom) {
    // let superclass begin the render
    Shader::BeginRender(geom);

    // Confirm that geometry has color and texture data
    MY_ASSERT(geom->HasColors());
    MY_ASSERT(mColorLoc >= 0);
    MY_ASSERT(geom->HasTexCoords());
    MY_ASSERT(mTexCoordLoc >= 0);

    // push color data
    glVertexAttribPointer(mColorLoc, 3, GL_FLOAT, GL_FALSE, geom->GetStride(),
                          BUFFER_OFFSET(geom->GetColorsOffset()));
    glEnableVertexAttribArray(mColorLoc);

    // push texture coordinates
    glVertexAttribPointer(mTexCoordLoc, 2, GL_FLOAT, GL_FALSE, geom->GetStride(),
                          BUFFER_OFFSET(geom->GetTexCoordsOffset()));
    glEnableVertexAttribArray(mTexCoordLoc);

    // set neutral tint color (white) as a default
    SetTintColor(1.0, 1.0, 1.0);

    // by default, no point light
    DisablePointLight();
}

const char* OurShader::GetVertShaderSource() {
    return OUR_VERTEX_SHADER_SOURCE;
}

const char* OurShader::GetFragShaderSource() { return OUR_FRAG_SHADER_SOURCE; }

const char* OurShader::GetShaderName() { return "OurShader"; }
