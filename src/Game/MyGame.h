#pragma once

#include "../Core/Game.h"

#include "Scenes/Level.h"

class MyGame : public Game {
public:
  MyGame()
      : Game(), mRegularShader(Shader::fromFile(
                    "res/shaders/regularShader/regular.vert",
                    "res/shaders/regularShader/regular.frag")) {}
  void Init() override {
    Game::Init();
    mLevel.pushCube(
        Transform(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)));

    std::string regular = "regular";
    Renderer.AddRegularShader(regular, &mRegularShader);

    SceneManager.AddScene("level", &mLevel);
    SceneManager.SetActiveScene("level");

    std::string cubeMapName = "cubeMap";

    // construct + store
    mCubeMaps.emplace_back(
        std::make_unique<CubeMap>("res/textures/cubemaps/skybox/right.jpg",
                                  "res/textures/cubemaps/skybox/left.jpg",
                                  "res/textures/cubemaps/skybox/top.jpg",
                                  "res/textures/cubemaps/skybox/bottom.jpg",
                                  "res/textures/cubemaps/skybox/front.jpg",
                                  "res/textures/cubemaps/skybox/back.jpg"));

    // get a valid pointer from the vector
    CubeMap *cubePtr = mCubeMaps.back().get();

    // register and activate
    Renderer.AddCubeMap(cubeMapName, cubePtr);
    Renderer.SetActiveCubeMap(cubeMapName);

    // Post processing shaders
    std::unique_ptr<Shader> ppShader = std::make_unique<Shader>(
        Shader::fromFile("res/shaders/ppShader/ppShader.vert",
                         "res/shaders/ppShader/ppShader.frag"));

    std::unique_ptr<Shader> ppShader2 = std::make_unique<Shader>(
        Shader::fromFile("res/shaders/ppShader/ppShader.vert",
                         "res/shaders/ppShader/ppShader2.frag"));

    mPostProcessingShaders.push_back(std::move(ppShader));
    mPostProcessingShaders.push_back(std::move(ppShader2));

    // Renderer.AddPostProcessingShader(mPostProcessingShaders.back().get());
    // Renderer.AddPostProcessingShader(mPostProcessingShaders.front().get());
  }

private:
  Level mLevel;
  Shader mRegularShader;
  std::vector<std::unique_ptr<Shader>> mPostProcessingShaders;
  std::vector<std::unique_ptr<CubeMap>> mCubeMaps;
};
