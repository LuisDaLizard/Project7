#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "texture.h"

class Framebuffer
{
private:
    unsigned int mFramebufferID;
    unsigned int mDepthBufferID;

    Texture mTexture;

    int mWidth, mHeight;

public:
    Framebuffer();
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer& operator=(Framebuffer&&) = delete;

    bool Create(int width, int height, bool depth);
    void Begin() const;
    void End(int width, int height);


    Texture &GetTexture();
};

#endif //FRAMEBUFFER_H
