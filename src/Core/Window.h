#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <functional>
#include <string>

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string &title);
  ~Window();

  void PollEvents();
  void SwapBuffers();
  bool ShouldClose() const;

  void UpdateWindowTitle(const std::string &newTitle);

  bool IsKeyPressed(int key) const;
  bool IsMouseButtonPressed(int button) const;
  GLFWwindow *GetNativeWindow() const;

  std::function<void(double, double)> mouseMoveCallback;
  std::function<void(int, int)> framebufferSizeCallback;

private:
  GLFWwindow *mWindow;

  static void MouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void FramebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};
