#include "mesh.h"

#include <glad/glad.h>

Mesh::Mesh()
    : mVAO(0), mVBO(0), mNumVertices(0)
{
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}

void Mesh::Create(const Vertex *vertices, int numVertices)
{
    mNumVertices = numVertices;

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, (long)(mNumVertices * sizeof(Vertex)), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, texture)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader, Material &material) const
{
    if (!mNumVertices)
        return;

    shader.Use();
    shader.UploadUniform("uMaterial.kDiffuse", material.kDiffuse);
    shader.UploadUniform("uMaterial.kAmbience", material.kAmbience);
    shader.UploadUniform("uMaterial.kSpecular", material.kSpecular);
    shader.UploadUniform("uMaterial.kShininess", material.kShininess);
    shader.UploadUniform("uMaterial.bDiffuse", material.bDiffuse);
    shader.UploadUniform("uMaterial.mTextureDiffuse", 0);
    shader.UploadUniform("uMaterial.bAmbience", material.bAmbience);
    shader.UploadUniform("uMaterial.mTextureAmbience", 1);
    shader.UploadUniform("uMaterial.bSpecular", material.bSpecular);
    shader.UploadUniform("uMaterial.mTextureSpecular", 2);

    if (material.bDiffuse)
        material.tDiffuse->Bind(0);
    if (material.bAmbience)
        material.tAmbience->Bind(1);
    if (material.bSpecular)
        material.tAmbience->Bind(2);

    glBindVertexArray(mVAO);

    glDrawArrays(GL_TRIANGLES, 0, mNumVertices);

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) const
{
    if (!mNumVertices)
        return;

    glBindVertexArray(mVAO);

    glDrawArrays(GL_TRIANGLES, 0, mNumVertices);

    glBindVertexArray(0);
}
