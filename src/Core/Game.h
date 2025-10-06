#pragma once

#include <glad/glad.h>

#include "Camera.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Window.h"

class Game {
private:
  Window mWindow;

public:
  Game();
  ~Game();

  virtual void Init();
  void Run();

  SceneManager SceneManager;
  Renderer Renderer;

private:
  Camera mCamera;

  // Timing
  float mLastFrame = 0.0f;
  float mDeltaTime = 0.0f;
  unsigned int mCounter = 0;

  // Mouse state
  double mLastX = 400.0; // screen center
  double mLastY = 300.0;
  bool mFirstMouse = true;

  glm::mat4 projection =
      glm::perspective(glm::radians(85.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  float mMouseSensitivity = 0.12f;
  // Input + callbacks
  void ProcessInput(float deltaTime);
  void OnMouseMove(double xpos, double ypos);

  // Game loop phases
  void Update(float deltaTime);
  void Draw();
};
