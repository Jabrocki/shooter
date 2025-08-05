#pragma once

#include "../OpenGL/Texture.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/VertexBuffer.h"
#include "../OpenGL/VertexBufferLayout.h"
#include "../OpenGL/VertexArray.h"

class Floor
{
public:
    Floor();
    ~Floor();

    void Draw();
private:
    VertexBuffer* m_VBO = nullptr;
    VertexBufferLayout* m_layout = nullptr;
    VertexArray* m_VAO = nullptr;

    Shader* m_shader = nullptr;
    Texture* m_texture = nullptr;
};

