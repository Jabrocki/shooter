#include "Window.h"
#include <cstdlib>
#include <iostream>
#include <string>

Window::Window(unsigned int width, unsigned int height,
               const std::string &title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    std::exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  if (!mWindow) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    std::exit(1);
  }

  glfwMakeContextCurrent(mWindow);
  glfwSetWindowUserPointer(mWindow, this);
  glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
  glfwSetCursorPosCallback(mWindow, MouseCallback);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    std::exit(1);
  }
}

Window::~Window() {
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

void Window::PollEvents() { glfwPollEvents(); }

void Window::SwapBuffers() { glfwSwapBuffers(mWindow); }

bool Window::ShouldClose() const { return glfwWindowShouldClose(mWindow); }

bool Window::IsKeyPressed(int key) const {
  return glfwGetKey(mWindow, key) == GLFW_PRESS;
}

bool Window::IsMouseButtonPressed(int button) const {
  return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
}

void Window::UpdateWindowTitle(const std::string &newTitle) {
  glfwSetWindowTitle(mWindow, newTitle.c_str());
}

GLFWwindow *Window::GetNativeWindow() const { return mWindow; }

void Window::FramebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  glViewport(0, 0, width, height);
}

void Window::MouseCallback(GLFWwindow *window, double xpos, double ypos) {
  auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win && win->mouseMoveCallback)
    win->mouseMoveCallback(xpos, ypos);
}
