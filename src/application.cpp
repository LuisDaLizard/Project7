#include "application.h"
#include "utils.h"

Application::Application(int width, int height, const char* modelFile)
    : mWidth(width), mHeight(height)
{
    if (!mModelShader.LoadFromSource(Shaders::ReflectionVS, Shaders::ReflectionFS))
        Utils::Error(1, "Unable to load model shaders.");

    if (!mPlaneShader.LoadFromSource(Shaders::EnvironmentVS, Shaders::EnvironmentFS))
        Utils::Error(1, "Unable to load environment shaders.");

    if (!mModel.LoadFromFile(modelFile))
        Utils::Error(1, "Unable to load model.");

    const char *files[6] = {"textures/cubemap/cubemap_posx.png",
                            "textures/cubemap/cubemap_negx.png",
                            "textures/cubemap/cubemap_posy.png",
                            "textures/cubemap/cubemap_negy.png",
                            "textures/cubemap/cubemap_posz.png",
                            "textures/cubemap/cubemap_negz.png" };

    if (!mEnvironmentMap.LoadCubemapFromFiles(files))
        Utils::Error(1, "Unable to load environment map.");

    mPlaneMesh.Create(Meshes::PlaneMeshVertices, 6);

    cyVec3f modelSize = mModel.GetSize();
    float modelScale = 1.0f / MAX(modelSize.x, MAX(modelSize.y, modelSize.z));

    mCameraTarget = {0, 0.25f, 0};
    mModelWorld = cyMatrix4f::Scale(modelScale) * cyMatrix4f::Translation({0, 0.5f, 0}) * cyMatrix4f::RotationX(-90 * DEG2RAD);
    mSkyboxWorld = cyMatrix4f::Scale(1) * cyMatrix4f::Translation({0, 0, 0.9999f});
    mPlaneWorld = cyMatrix4f::Scale(1) * cyMatrix4f::RotationX(90 * DEG2RAD);
}

void Application::Update()
{
    if (mMouseLeftDown)
    {
        cyVec2d mouseDelta;
        mouseDelta.x = mMouse.x - mPrevMouse.x;
        mouseDelta.y = mMouse.y - mPrevMouse.y;

        mModelYaw -= (float)(mouseDelta.x * mMouseSensitivity);
        mModelPitch += (float)(mouseDelta.y * mMouseSensitivity);
        mModelPitch = CLAMP(-89.0f, mModelPitch, 89.0f);

        mPrevMouse = mMouse;
    }

    if (mMouseRightDown)
    {
        double mouseYDelta = mMouse.y - mPrevMouse.y;

        mModelRadius += (float)(mouseYDelta * mZoomSensitivity);
        mModelRadius = MAX(mModelRadius, 0.01f);

        mPrevMouse = mMouse;
    }

    /* Calculate Model Scene Camera Position */
    mCamera.x = sin(mModelYaw * DEG2RAD) * cos(mModelPitch * DEG2RAD) * mModelRadius;
    mCamera.y = sin(mModelPitch * DEG2RAD) * mModelRadius + mCameraTarget.y;
    mCamera.z = cos(mModelYaw * DEG2RAD) * cos(mModelPitch * DEG2RAD) * mModelRadius;
    mModelView.SetView(mCamera, mCameraTarget, {0, 1, 0});

    /* Calculate Light Position */
    mLightRotation = (float)glfwGetTime();
    mLight.x = sin(mLightRotation) * mModelRadius;
    mLight.y = mModelRadius;
    mLight.z = cos(mLightRotation) * mModelRadius;
}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelShader.Use();
    mModelShader.UploadUniform("uProjection", mModelProjection);
    mModelShader.UploadUniform("uView", mModelView);
    mModelShader.UploadUniform("uModel", mModelWorld);
    mModelShader.UploadUniform("uLightPos", mLight);
    mModelShader.UploadUniform("uViewPos", mCamera);
    mModelShader.UploadUniform("uEnvironment", 4);
    mEnvironmentMap.Bind(4);

    mModel.Draw(mModelShader);

    mModelShader.UploadUniform("uModel", mPlaneWorld);
    mPlaneMesh.Draw(mModelShader, mPlaneMaterial);

    glDepthMask(GL_FALSE);

    mPlaneShader.Use();
    mPlaneShader.UploadUniform("uProjection", mModelProjection);
    mPlaneShader.UploadUniform("uView", mModelView);
    mPlaneShader.UploadUniform("uModel", mSkyboxWorld);

    mEnvironmentMap.Bind(0);

    mPlaneMesh.Draw(mPlaneShader, mPlaneMaterial);

    glDepthMask(GL_TRUE);
}

void Application::KeyCallback(GLFWwindow *handle, int key, int scancode, int action, int mods)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    Application *pApp = (Application *)p;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        Utils::Error(0, "Exiting");
}

void Application::ResizeCallback(GLFWwindow *handle, int width, int height)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    Application *pApp = (Application *)p;

    glViewport(0, 0, width, height);
    pApp->mModelProjection.SetPerspective(45.0f, (float)width / (float)height, 0.01f, 1000.0f);
}

void Application::CursorPosCallback(GLFWwindow *handle, double x, double y)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    Application *pApp = (Application *)p;

    pApp->mPrevMouse = pApp->mMouse;
    pApp->mMouse.x = x;
    pApp->mMouse.y = y;
}

void Application::MouseButtonCallback(GLFWwindow *handle, int button, int action, int mods)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    Application *pApp = (Application *)p;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
        pApp->mMouseLeftDown = action;

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        pApp->mMouseRightDown = action;
}
