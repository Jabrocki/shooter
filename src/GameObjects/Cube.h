#pragma once

#include <glm/glm.hpp>


#include "../OpenGL/Texture.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/VertexBuffer.h"
#include "../OpenGL/VertexBufferLayout.h"
#include "../OpenGL/VertexArray.h"


class Cube
{
public:
    Cube();
    ~Cube();

    void Update(float deltaTime);
    void Draw(glm::mat4 view, glm::mat4 projection);
private:
    glm::vec3 m_Position = glm::vec3(0.0f, 5.0f, -3.0f);
    glm::vec3 m_Velocity = glm::vec3(0.0, 0.0, 0.0);
    glm::mat4 m_Model = glm::mat4(1.0f);
    
    float m_Gravity = -9.81f; // Gravity acceleration (m/s²)
    float m_GroundLevel = 0.0f; // Floor level

    VertexBuffer* m_VBO = nullptr;
    VertexBufferLayout* m_layout = nullptr;
    VertexArray* m_VAO = nullptr;

    Shader* m_shader = nullptr;
    Texture* m_texture = nullptr;
};

