#pragma once

// OpenGL
#include "../../OpenGL/Shader.h"
#include "../Components/Transform.h"

#include <string>

class Renderable {
public:
  Renderable(const std::string &objType) : type(objType) {};
  virtual void Draw(Shader &shader) = 0;
  const std::string type;

  Transform *transform;
};
