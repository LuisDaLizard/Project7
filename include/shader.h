#ifndef SHADER_H
#define SHADER_H

#include <cyVector.h>
#include <cyMatrix.h>

class Shader
{
private:
    unsigned int mProgramID;

public:
    Shader();
    ~Shader();

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

    void Use() const;
    bool LoadFromSource(const char *vertSrc, const char *fragSrc);
    bool LoadFromFile(const char *vertFileName, const char *fragFileName);

    bool UploadUniform(const char *name, bool value) const;
    bool UploadUniform(const char *name, int value) const;
    bool UploadUniform(const char *name, int *values, int count) const;
    bool UploadUniform(const char *name, float value) const;
    bool UploadUniform(const char *name, float *values, int count) const;
    bool UploadUniform(const char *name, cyVec2f value) const;
    bool UploadUniform(const char *name, cyVec3f value) const;
    bool UploadUniform(const char *name, cyVec4f value) const;
    bool UploadUniform(const char *name, cyMatrix4f value) const;
};

namespace Shaders
{
    static const char *ShadowVS = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uLightProjection;
uniform mat4 uLightView;
uniform mat4 uLightTransform;

out vec3 fPosition;
out vec4 fLightViewPosition;
out vec3 fNormal;
out vec2 fTexCoords;

void main()
{
    fPosition = (uModel * vec4(aPosition, 1)).xyz;
    fLightViewPosition = uLightTransform * (uLightProjection * uLightView * uModel) * vec4(aPosition, 1);
    fNormal = mat3(transpose(inverse(uModel))) * aNormal;
    fTexCoords = aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1);
}
)";

    static const char *ShadowFS = R"(
#version 330 core

struct Material
{
    vec3 kAmbience;
    vec3 kDiffuse;
    vec3 kSpecular;
    float kShininess;

    bool bDiffuse;
    sampler2D mTextureDiffuse;

    bool bAmbience;
    sampler2D mTextureAmbience;

    bool bSpecular;
    sampler2D mTextureSpecular;
};

out vec4 oColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2DShadow uShadowMap;
uniform Material uMaterial;

in vec3 fPosition;
in vec4 fLightViewPosition;
in vec3 fNormal;
in vec2 fTexCoords;

float diffuse(vec3 lightDir, vec3 normal)
{
    return max(dot(lightDir, normal), 0.0);
}

float specular(vec3 lightDir, vec3 viewDir, vec3 position, vec3 normal)
{
    vec3 halfway = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfway), 0.0), uMaterial.kShininess);
}

void main()
{
    vec3 position = fPosition.xyz;
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(uLightPos - fPosition);
    vec3 viewDir = normalize(uViewPos - fPosition);

    vec4 lightDiffuse = vec4(vec3(diffuse(lightDir, normal)), 1);
    vec4 lightAmbience = vec4(0, 0, 0, 1);
    vec4 lightSpecular = vec4(vec3(specular(lightDir, viewDir, position, normal)), 1);

    if (uMaterial.bDiffuse)
        lightDiffuse *= texture(uMaterial.mTextureDiffuse, fTexCoords);
    else
        lightDiffuse *= vec4(uMaterial.kDiffuse, 1);

    if (uMaterial.bAmbience)
        lightAmbience *= texture(uMaterial.mTextureAmbience, fTexCoords);
    else
        lightAmbience *= vec4(uMaterial.kAmbience, 1);

    if (uMaterial.bSpecular)
        lightSpecular *= texture(uMaterial.mTextureSpecular, fTexCoords);
    else
        lightSpecular *= vec4(uMaterial.kSpecular, 1);

    oColor = (lightAmbience + lightDiffuse + lightSpecular);

    oColor *= textureProj(uShadowMap, fLightViewPosition);
}
)";

    static const char *DepthVS = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1);
}
)";

    static const char *DepthFS = R"(
)";


    static const char *BlinnVS = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 fNormal;
out vec3 fPosition;
out vec2 fTexCoords;

void main()
{
    fPosition = (uModel * vec4(aPosition, 1)).xyz;
    fNormal = mat3(transpose(inverse(uModel))) * aNormal;
    fTexCoords = aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1);
}
)";

    static const char *BlinnFS = R"(
#version 330 core

struct Material
{
    vec3 kAmbience;
    vec3 kDiffuse;
    vec3 kSpecular;
    float kShininess;

    bool bDiffuse;
    sampler2D mTextureDiffuse;

    bool bAmbience;
    sampler2D mTextureAmbience;

    bool bSpecular;
    sampler2D mTextureSpecular;
};

out vec4 oColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform Material uMaterial;

in vec3 fNormal;
in vec3 fPosition;
in vec2 fTexCoords;

float diffuse(vec3 lightDir, vec3 normal)
{
    return max(dot(lightDir, normal), 0.0);
}

float specular(vec3 lightDir, vec3 viewDir, vec3 position, vec3 normal)
{
    vec3 halfway = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfway), 0.0), uMaterial.kShininess);
}

void main()
{
    vec3 position = fPosition.xyz;
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(uLightPos - fPosition);
    vec3 viewDir = normalize(uViewPos - fPosition);

    vec4 lightDiffuse = vec4(vec3(diffuse(lightDir, normal)), 1);
    vec4 lightAmbience = vec4(0.1, 0.1, 0.1, 1);
    vec4 lightSpecular = vec4(vec3(specular(lightDir, viewDir, position, normal)), 1);

    if (uMaterial.bDiffuse)
        lightDiffuse *= texture(uMaterial.mTextureDiffuse, fTexCoords);
    else
        lightDiffuse *= vec4(uMaterial.kDiffuse, 1);

    if (uMaterial.bAmbience)
        lightAmbience *= texture(uMaterial.mTextureAmbience, fTexCoords);
    else
        lightAmbience *= vec4(uMaterial.kAmbience, 1);

    if (uMaterial.bSpecular)
        lightSpecular *= texture(uMaterial.mTextureSpecular, fTexCoords);
    else
        lightSpecular *= vec4(uMaterial.kSpecular, 1);

    oColor = lightAmbience + lightDiffuse + lightSpecular;
}
)";
}


#endif //SHADER_H