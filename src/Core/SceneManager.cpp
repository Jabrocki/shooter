#include "SceneManager.h"

void SceneManager::AddScene(const std::string &name, Scene *scene) {
  mScenes[name] = scene;
}

void SceneManager::RemoveScene(const std::string &name) { mScenes.erase(name); }

Scene *SceneManager::GetActiveScene() {
  if (mScenes.find(mActiveScene) != mScenes.end()) {
    return mScenes[mActiveScene];
  }
  return nullptr;
}

void SceneManager::SetActiveScene(const std::string &name) {
  if (mScenes.find(name) != mScenes.end()) {
    mActiveScene = name;
  }
}
