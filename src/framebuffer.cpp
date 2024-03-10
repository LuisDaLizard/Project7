#include "framebuffer.h"

#include <glad/glad.h>

Framebuffer::Framebuffer()
    : mFramebufferID(0), mTexture(), mDepthbufferID(0), mWidth(0), mHeight(0)
{

}

Framebuffer::~Framebuffer()
{
    if (mFramebufferID)
        glDeleteFramebuffers(1, &mFramebufferID);

    if (mDepthbufferID)
        glDeleteRenderbuffers(1, &mDepthbufferID);
}

bool Framebuffer::Create(int width, int height, bool depth)
{
    mWidth = width;
    mHeight = height;

    glGenFramebuffers(1, &mFramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

    mTexture.LoadFromData(mWidth, mHeight, 3, nullptr);

    if (depth)
    {
        glGenRenderbuffers(1, &mDepthbufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthbufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthbufferID);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture.GetID(), 0);

    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

bool Framebuffer::CreateDepthOnly(int width, int height)
{
    mWidth = width;
    mHeight = height;

    glGenFramebuffers(1, &mFramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

    mTexture.LoadDepthFromData(mWidth, mHeight, nullptr);
    mDepthbufferID = mTexture.GetID();

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthbufferID, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void Framebuffer::Begin() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
    glViewport(0, 0, mWidth, mHeight);
}

void Framebuffer::End(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
}

Texture &Framebuffer::GetTexture()
{
    return mTexture;
}