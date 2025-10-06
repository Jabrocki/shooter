#include "Game.h"

// OpenGL
#include "GLFW/glfw3.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <string>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// TODO: refactor renderer init
Game::Game() : mWindow(SCR_WIDTH, SCR_HEIGHT, "Game") {}

Game::~Game() {}

void Game::Init() {
  // Mouse input
  mWindow.mouseMoveCallback = [this](double xpos, double ypos) {
    this->OnMouseMove(xpos, ypos);
  };

  mWindow.framebufferSizeCallback = [this](float width, float height) {
    // Update projection matrix
    projection = glm::perspective(
        glm::radians(85.0f), static_cast<float>(width) / height, 0.1f, 100.0f);

    // Resize framebuffers
    this->Renderer.FBO.Resize(width, height);
    this->Renderer.pingPongFBO[0].Resize(width, height);
    this->Renderer.pingPongFBO[1].Resize(width, height);
  };

  // Capture mouse
  // glfwSetInputMode(mWindow.GetNativeWindow(), GLFW_CURSOR,
  //                 GLFW_CURSOR_DISABLED);
}

void Game::Run() {
  while (!mWindow.ShouldClose()) {
    // Move glftGetTime to window class it is GLFW wrappper
    float currentTime = glfwGetTime();
    mDeltaTime = currentTime - mLastFrame;
    mLastFrame = currentTime;

    ProcessInput(mDeltaTime);
    Update(mDeltaTime);
    Draw();

    mWindow.SwapBuffers();
    mWindow.PollEvents();
  }
}

void Game::ProcessInput(float deltaTime) {
  // TODO: move to input manager
  if (mWindow.IsKeyPressed(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(mWindow.GetNativeWindow(), true);

  glm::vec3 direction(0.0f);
  if (mWindow.IsKeyPressed(GLFW_KEY_W))
    direction += mCamera.GetFront();
  if (mWindow.IsKeyPressed(GLFW_KEY_S))
    direction -= mCamera.GetFront();
  if (mWindow.IsKeyPressed(GLFW_KEY_A))
    direction -=
        glm::normalize(glm::cross(mCamera.GetFront(), glm::vec3(0, 1, 0)));
  if (mWindow.IsKeyPressed(GLFW_KEY_D))
    direction +=
        glm::normalize(glm::cross(mCamera.GetFront(), glm::vec3(0, 1, 0)));
  if (glm::length(direction) > 0.0f)
    mCamera.ProcessKeyboardMovement(glm::normalize(direction), deltaTime);
}

void Game::OnMouseMove(double xpos, double ypos) {
  if (mFirstMouse) {
    mLastX = xpos;
    mLastY = ypos;
    mFirstMouse = false;
  }

  float xoffset = xpos - mLastX;
  float yoffset = mLastY - ypos; // reversed: y goes from bottom to top

  mLastX = xpos;
  mLastY = ypos;

  mCamera.ProcessMouseMovement(xoffset, yoffset);
}

void Game::Update(float deltaTime) {
  SceneManager.GetActiveScene()->Update(deltaTime);
}

void Game::Draw() {
  Renderer.Draw(*SceneManager.GetActiveScene(), mCamera.GetViewMatrix(),
                projection, mCamera.GetPosition());
}
