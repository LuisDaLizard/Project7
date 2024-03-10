#include "shader.h"

#include <glad/glad.h>
#include "utils.h"

Shader::Shader()
    : mProgramID(0)
{
}

Shader::~Shader()
{
    glDeleteProgram(mProgramID);
}

void Shader::Use() const
{
    glUseProgram(mProgramID);
}

bool Shader::LoadFromSource(const char *vertSrc, const char *fragSrc)
{
    int success;
    char infoLog[512];
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertSrc, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        Utils::Info(infoLog);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragSrc, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        Utils::Info(infoLog);
    }

    mProgramID = glCreateProgram();
    glAttachShader(mProgramID, vertex);
    glAttachShader(mProgramID, fragment);
    glLinkProgram(mProgramID);

    glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(mProgramID, 512, nullptr, infoLog);
        Utils::Info(infoLog);
        return false;
    }

    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

bool Shader::LoadFromFile(const char *vertFileName, const char *fragFileName)
{
    char *vertexSrc = Utils::ReadFile(vertFileName);
    char *fragmentSrc = Utils::ReadFile(fragFileName);

    if (!vertexSrc || !fragmentSrc)
        return false;

    bool result = LoadFromSource(vertexSrc, fragmentSrc);

    // Clean up
    delete[] vertexSrc;
    delete[] fragmentSrc;

    return result;
}

bool Shader::UploadUniform(const char *name, bool value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform1i(location, value);

    return true;
}

bool Shader::UploadUniform(const char *name, int value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform1i(location, value);

    return true;
}

bool Shader::UploadUniform(const char *name, int *values, int count) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform1iv(location, count, values);

    return true;
}

bool Shader::UploadUniform(const char *name, float value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform1f(location, value);

    return true;
}

bool Shader::UploadUniform(const char *name, float *values, int count) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform1fv(location, count, values);

    return true;
}

bool Shader::UploadUniform(const char *name, cyVec2f value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform2f(location, value.x, value.y);

    return true;
}

bool Shader::UploadUniform(const char *name, cyVec3f value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform3f(location, value.x, value.y, value.z);

    return true;
}

bool Shader::UploadUniform(const char *name, cyVec4f value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniform4f(location, value.x, value.y, value.z, value.w);

    return true;
}

bool Shader::UploadUniform(const char *name, cyMatrix4f value) const
{
    int location = glGetUniformLocation(mProgramID, name);

    if (location == -1)
        return false;

    glUniformMatrix4fv(location, 1, false, (float *)&value);

    return true;
}
