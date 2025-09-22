#pragma once

// Game components
#include "Scene.h"

// STL
#include <string>
#include <unordered_map>

class SceneManager {
public:
  SceneManager() {};
  ~SceneManager() {}
  void AddScene(const std::string &name, Scene *scene);
  void RemoveScene(const std::string &name);

  void SetActiveScene(const std::string &name);
  Scene *GetActiveScene();

private:
  std::unordered_map<std::string, Scene *> mScenes;
  std::string mActiveScene;
};
