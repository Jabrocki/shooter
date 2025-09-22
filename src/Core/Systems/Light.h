#pragma once

// GLM
#include <glm/glm.hpp>

// Componets
#include "../Components/Transform.h"

/*
Light struct inside shader
 struct Light {
    vec3 position;

    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
*/

struct Light {
  Transform *transform;

  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
