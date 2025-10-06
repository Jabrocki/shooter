#pragma once

// STD
#include <vector>

// Systems
#include "Systems/Light.h"
#include "Systems/Renderable.h"
#include "Systems/Updatable.h"

class Scene {
public:
  // TODO: implement deconstructor and a constructor
  void Update(float deltaTime);
  std::vector<Renderable *> opaqueObjects;
  std::vector<Renderable *> transparentObjects;
  std::vector<Updatable *> updatableObjects;
  std::vector<Light *> lights;

  // Future: split that into clusters
  std::vector<Light *> getClosestLights(glm::vec3 objPosition,
                                        unsigned int count);
};
