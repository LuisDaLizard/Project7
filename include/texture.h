#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
private:
    unsigned int mTextureID;
    unsigned int mTextureType;

public:
    Texture();
    ~Texture();

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    bool LoadFromData(int width, int height, int channels, void *data);
    bool LoadFromFile(const char *fileName);
    bool LoadCubemapFromData(int width, int height, int channels, void *pX, void *nX, void *pY, void *nY, void *pZ, void *nZ);
    bool LoadCubemapFromFiles(const char *files[6]);
    bool LoadDepthFromData(int width, int height, void *data);
    void Bind(unsigned int slot = 0) const;

    unsigned int GetID() const;
};

#endif //TEXTURE_H