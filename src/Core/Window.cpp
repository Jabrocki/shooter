#include "Window.h"
#include <iostream>
#include <cstdlib>

Window::Window(unsigned int width, unsigned int height, const std::string& title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    
    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        std::exit(1);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_Window, MouseCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

bool Window::IsKeyPressed(int key) const
{
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Window::IsMouseButtonPressed(int button) const
{
    return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
}

GLFWwindow* Window::GetNativeWindow() const
{
    return m_Window;
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->mouseMoveCallback)
        win->mouseMoveCallback(xpos, ypos);
}
