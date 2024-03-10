#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>
#include "model.h"
#include "framebuffer.h"

class Application
{
private:
    int mWidth, mHeight;
    Model mModel;
    Mesh mPlaneMesh;
    Mesh::Material mPlaneMaterial;
    Shader mModelShader;
    Texture mEnvironmentMap;

    cyMatrix4f mPlaneWorld;
    cyMatrix4f mModelProjection, mModelView, mModelWorld;
    cyVec3f mCamera, mCameraTarget, mLight;
    cyVec2d mMouse = {0, 0}, mPrevMouse = {0, 0};

    float mModelYaw = 0, mModelPitch = 0, mModelRadius = 1;
    const float mMouseSensitivity = 0.1f;
    const float mZoomSensitivity = 0.01f;
    float mLightRotation = 0;

    bool mMouseLeftDown = false, mMouseRightDown = false;

public:
    Application(int width, int height, const char* modelFile);
    ~Application() = default;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    void Update();
    void Draw();

    static void KeyCallback(GLFWwindow *handle, int key, int scancode, int action, int mods);
    static void ResizeCallback(GLFWwindow *handle, int width, int height);
    static void CursorPosCallback(GLFWwindow *handle, double x, double y);
    static void MouseButtonCallback(GLFWwindow *handle, int button, int action, int mods);
};

#endif //APPLICATION_H
