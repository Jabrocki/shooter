#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class Window
{
public:
        
    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;

    bool IsKeyPressed(int key) const;
    bool IsMouseButtonPressed(int button) const;
    GLFWwindow* GetNativeWindow() const;

    std::function<void(double, double)> mouseMoveCallback;
private:
    GLFWwindow* m_Window;

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};





