#pragma once

#include <glm/glm.hpp>

// ECS
#include "../ECS/Components/Transform.h"

// OpenGL
#include "../OpenGL/Texture.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/VertexBuffer.h"
#include "../OpenGL/VertexBufferLayout.h"
#include "../OpenGL/VertexArray.h"

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
    VertexBuffer m_VBO;
    VertexBufferLayout m_layout;
    VertexArray m_VAO;

    Texture m_diffuse;
    Texture m_specular;
};
