#pragma once

#include "../Core/Game.h"

#include "Scenes/Level.h"

class MyGame : public Game {
public:
  MyGame()
      : Game(), mRegularShader("res/shaders/regularShader/regular.vert",
                               "res/shaders/regularShader/regular.frag") {}
  void Init() override {
    Game::Init();
    mLevel.pushCube(
        Transform(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)));

    std::string regular = "regular";
    Renderer.AddShader(regular, &mRegularShader);

    SceneManager.AddScene("level", &mLevel);
    SceneManager.SetActiveScene("level");
  }

private:
  Level mLevel;
  Shader mRegularShader;
};
