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

    void Draw();
    Shader* shader = nullptr;
private:
    glm::vec3 m_Position = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 m_Model = glm::mat4(1.0f);

    VertexBuffer* m_VBO = nullptr;
    VertexBufferLayout* m_layout = nullptr;
    VertexArray* m_VAO = nullptr;

    
    Texture* m_texture = nullptr;
};

