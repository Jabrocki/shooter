#pragma once

#include <glm/glm.hpp>


#include "../OpenGL/Texture.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/VertexBuffer.h"
#include "../OpenGL/VertexBufferLayout.h"
#include "../OpenGL/VertexArray.h"

class Light
{
public:
    Light(/* args */);
    ~Light();

    void Update(float deltaTime);
    void Draw();
    glm::vec3 getPosition() {return m_Position;}
private:
    glm::vec3 m_Position = glm::vec3(0.0f, 1.0f, 2.0f);
    glm::mat4 m_Model = glm::mat4(1.0f);

    float m_AngularSpeed = 0.5f;
    float m_Angle = 0.0f;

    VertexBuffer* m_VBO = nullptr;
    VertexBufferLayout* m_layout = nullptr;
    VertexArray* m_VAO = nullptr;
    Shader* m_shader = nullptr;
};
