#include "utils.h"
#include "application.h"

Application::Application(int width, int height, const char* modelFile)
    : mWidth(width), mHeight(height)
{
    if (!mModelShader.LoadFromSource(Shaders::BlinnVS, Shaders::BlinnFS))
        Utils::Error(1, "Unable to load model shaders.");

    if (!mDepthViewShader.LoadFromSource(Shaders::DisplayDepthVS, Shaders::DisplayDepthFS))
        Utils::Error(1, "Unable to load depth view shaders.");

    if (!mDepthShader.LoadFromSource(Shaders::DepthVS, Shaders::DepthFS))
        Utils::Error(1, "Unable to load depth shaders.");

    if (!mModel.LoadFromFile(modelFile))
        Utils::Error(1, "Unable to load model.");

    if (!mDepthbuffer.CreateDepthOnly(1024, 1024))
        Utils::Error(1, "Unable to create depthbuffer.");

    mPlaneMesh.Create(Meshes::PlaneMeshVertices, 6);
    mPlaneMaterial.bAmbience = false;
    mPlaneMaterial.bDiffuse = false;
    mPlaneMaterial.bSpecular = false;
    mPlaneMaterial.kShininess = 100;
    mPlaneMaterial.kAmbience = cyVec3f(0.5f, 0.5f, 0.5f);
    mPlaneMaterial.kDiffuse = cyVec3f(0.7f, 0.7f, 0.7f);
    mPlaneMaterial.kSpecular = cyVec3f(1, 1, 1);

    mDepthViewMaterial.bAmbience = false;
    mDepthViewMaterial.bDiffuse = true;
    mDepthViewMaterial.bSpecular = false;
    mDepthViewMaterial.tDiffuse = &mDepthbuffer.GetTexture();

    cyVec3f modelSize = mModel.GetSize();
    float modelScale = 1.0f / MAX(modelSize.x, MAX(modelSize.y, modelSize.z));

    mCameraTarget = {0, 0.25f, 0};
    mModelWorld = cyMatrix4f::Scale(modelScale) * cyMatrix4f::Translation({0, 0.5f, 0}) * cyMatrix4f::RotationX(-90 * DEG2RAD);
    mPlaneWorld = cyMatrix4f::Scale(2) * cyMatrix4f::RotationX(90 * DEG2RAD);
    mDepthViewWorld = cyMatrix4f::Scale(0.25f) * cyMatrix4f::Translation({3, 3, 0});
    mDepthProjection = cyMatrix4f::Perspective(90 * DEG2RADF, 1, 1, 9);
    mDepthView = cyMatrix4f::View(mLight, cyVec3f(0, 0, 0), cyVec3f(0, 1, 0));
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
    mCamera.x = sinf(mModelYaw * DEG2RADF) * cosf(mModelPitch * DEG2RADF) * mModelRadius;
    mCamera.y = sinf(mModelPitch * DEG2RADF) * mModelRadius + mCameraTarget.y;
    mCamera.z = cosf(mModelYaw * DEG2RADF) * cosf(mModelPitch * DEG2RADF) * mModelRadius;
    mModelView.SetView(mCamera, mCameraTarget, {0, 1, 0});

    /* Calculate Light Position */
    mLightRotation = (float)glfwGetTime();
    mLight.x = sinf(mLightRotation) * 2;
    mLight.y = 2;
    mLight.z = cosf(mLightRotation) * 2;
    mDepthView.SetView(mLight, cyVec3f(0, 0, 0), cyVec3f(0, 1, 0));
}

void Application::Draw()
{
    mDepthbuffer.Begin();
    glClear(GL_DEPTH_BUFFER_BIT);

    mDepthShader.Use();
    mDepthShader.UploadUniform("uProjection", mDepthProjection);
    mDepthShader.UploadUniform("uView", mDepthView);
    mDepthShader.UploadUniform("uModel", mModelWorld);
    mModel.Draw(mDepthShader, false);

    mDepthShader.UploadUniform("uModel", mPlaneWorld);
    mPlaneMesh.Draw(mDepthShader, mPlaneMaterial);

    mDepthbuffer.End(mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelShader.Use();
    mModelShader.UploadUniform("uProjection", mModelProjection);
    mModelShader.UploadUniform("uView", mModelView);
    mModelShader.UploadUniform("uModel", mModelWorld);
    mModelShader.UploadUniform("uLightPos", mLight);
    mModelShader.UploadUniform("uViewPos", mCamera);
    mModel.Draw(mModelShader, true);

    mModelShader.UploadUniform("uModel", mPlaneWorld);
    mPlaneMesh.Draw(mModelShader, mPlaneMaterial);

    mDepthViewShader.Use();
    mDepthViewShader.UploadUniform("uModel", mDepthViewWorld);
    mPlaneMesh.Draw(mDepthViewShader, mDepthViewMaterial);
}

void Application::KeyCallback(GLFWwindow *, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        Utils::Error(0, "Exiting");
}

void Application::ResizeCallback(GLFWwindow *handle, int width, int height)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    auto *pApp = (Application *)p;

    glViewport(0, 0, width, height);
    pApp->mModelProjection.SetPerspective(45.0f, (float)width / (float)height, 0.01f, 1000.0f);
}

void Application::CursorPosCallback(GLFWwindow *handle, double x, double y)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    auto *pApp = (Application *)p;

    pApp->mPrevMouse = pApp->mMouse;
    pApp->mMouse.x = x;
    pApp->mMouse.y = y;
}

void Application::MouseButtonCallback(GLFWwindow *handle, int button, int action, int)
{
    void *p = glfwGetWindowUserPointer(handle);
    if (!p) return;

    auto *pApp = (Application *)p;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
        pApp->mMouseLeftDown = action;

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        pApp->mMouseRightDown = action;
}
