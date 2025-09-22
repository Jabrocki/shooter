#include "Scene.h"

// STM
#include <algorithm>
#include <vector>

void Scene::Update(float deltaTime) {
  for (auto *updatableObj : updatableObjects)
    updatableObj->Update(deltaTime);
}

std::vector<Light *> Scene::getClosestLights(glm::vec3 objPosition,
                                             unsigned int count) {
  std::vector<Light *> closestLights = lights;

  if (closestLights.size() > count) {
    std::nth_element(
        closestLights.begin(), closestLights.begin() + count,
        closestLights.end(), [objPosition](Light *a, Light *b) {
          float distA = glm::length(a->transform->GetPosition() - objPosition);
          float distB = glm::length(b->transform->GetPosition() - objPosition);
          return distA < distB;
        });
    closestLights.resize(count);
  }
  return closestLights;
}
