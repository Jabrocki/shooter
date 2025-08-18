#pragma once

#include <glm/glm.hpp>

// ECS
#include "../ECS/Components/Transform.h"
#include "../ECS/Components/Mesh.h"

// OpenGL
#include "../OpenGL/Texture.h"
#include "../OpenGL/Shader.h"

class Cube
{
public:
    Cube(
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        Shader *shader
    );

    ~Cube() {};

    void Draw();

    Shader *shader = nullptr;

private:
    Transform m_Transform;
    Mesh m_mesh;
};

