#pragma once

// Systems
#include "../../Core/Systems/MeshRenderable.h"

// Components
#include "../../Core/Components/Transform.h"

class Cube {
public:
  Cube(Transform initial);
  Transform transform;
  MeshRenderable renderable;
};
