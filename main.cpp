/*
 * CS 5610
 * Project 7 - Shadow Mapping
 * Luis Tadeo Sanchez
 * March 12, 2024
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "application.h"

GLFWwindow *window;

int main(int argc, char **argv)
{
    if (argc < 2)
        Utils::Error(1, "Missing required arguments. Correct usage: Project7 <obj_path>");
    
    if (!glfwInit())
        Utils::Error(1, "Unable to initialize GLFW");

    /* Create GLFW window */
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    window = glfwCreateWindow(800, 600, "Project 7", nullptr, nullptr);

    if (!window)
        Utils::Error(1, "Unable to create window");

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0);

    /* Initialize GLEW */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        Utils::Error(1, "Failed to initialize GLEW");

    /* Setup OpenGL */
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    Application app(width, height, argv[1]);

    glfwSetWindowUserPointer(window, &app);
    glfwSetKeyCallback(window, Application::KeyCallback);
    glfwSetFramebufferSizeCallback(window, Application::ResizeCallback);
    glfwSetCursorPosCallback(window, Application::CursorPosCallback);
    glfwSetMouseButtonCallback(window, Application::MouseButtonCallback);
    Application::ResizeCallback(window, width, height);

    while(!glfwWindowShouldClose(window))
    {
        app.Update();

        app.Draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
