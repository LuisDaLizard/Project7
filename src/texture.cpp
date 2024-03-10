#include "texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
    : mTextureID(0)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureID);
}

bool Texture::LoadFromData(int width, int height, int channels, void *data)
{
    mTextureType = GL_TEXTURE_2D;

    glGenTextures(1, &mTextureID);
    glBindTexture(mTextureType, mTextureID);

    glTexParameteri(mTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(mTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch(channels)
    {
        case 3:
            glTexImage2D(mTextureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(mTextureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            return false;
    }

    return true;
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(mTextureType, mTextureID);
}

bool Texture::LoadFromFile(const char *fileName)
{
    int width, height, channels;
    unsigned char *data = stbi_load(fileName, &width, &height, &channels, 0);

    if (!data)
        return false;

    bool result = LoadFromData(width, height, channels, data);

    stbi_image_free(data);

    return result;
}

unsigned int Texture::GetID() const
{
    return mTextureID;
}

bool Texture::LoadCubemapFromData(int width, int height, int channels, void *pX, void *nX, void *pY, void *nY, void *pZ, void *nZ)
{
    mTextureType = GL_TEXTURE_CUBE_MAP;

    glGenTextures(1, &mTextureID);
    glBindTexture(mTextureType, mTextureID);

    glTexParameteri(mTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(mTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch(channels)
    {
        case 3:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pX);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nX);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pY);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nY);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pZ);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nZ);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pX);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nX);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pY);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nY);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pZ);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nZ);
            break;
        default:
            return false;
    }

    return true;
}

bool Texture::LoadCubemapFromFiles(const char **files)
{
    int width, height, channels;
    unsigned char *pXData = stbi_load(files[0], &width, &height, &channels, 0);
    unsigned char *nXData = stbi_load(files[1], &width, &height, &channels, 0);
    unsigned char *pYData = stbi_load(files[2], &width, &height, &channels, 0);
    unsigned char *nYData = stbi_load(files[3], &width, &height, &channels, 0);
    unsigned char *pZData = stbi_load(files[4], &width, &height, &channels, 0);
    unsigned char *nZData = stbi_load(files[5], &width, &height, &channels, 0);

    if (!pXData || !nXData || !pYData || !nYData || !pZData || !nZData)
        return false;

    bool result = LoadCubemapFromData(width, height, channels, pXData, nXData, pYData, nYData, pZData, nZData);

    stbi_image_free(pXData);
    stbi_image_free(nXData);
    stbi_image_free(pYData);
    stbi_image_free(nYData);
    stbi_image_free(pZData);
    stbi_image_free(nZData);

    return result;
}

bool Texture::LoadDepthFromData(int width, int height, void *data)
{
    mTextureType = GL_TEXTURE_2D;

    glGenTextures(1, &mTextureID);
    glBindTexture(mTextureType, mTextureID);

    glTexParameteri(mTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(mTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);

    glBindTexture(mTextureType, 0);

    return true;
}
