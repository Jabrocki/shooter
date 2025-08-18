#include "Floor.h"

#include <glm/ext/matrix_clip_space.hpp>

//Core
#include "../Core/Camera.h"


//OpenGL
#include "../OpenGL/Debug.h"


Floor::Floor()
{
    float floorVertices[] = {
    // positions          // texture coords
        -50.0f, 0.0f,  50.0f,  0.0f, 100.0f,
        50.0f, 0.0f,  50.0f,  100.0f, 100.0f,
        -50.0f, 0.0f, -50.0f,  0.0f, 0.0f,

        -50.0f, 0.0f, -50.0f,  0.0f, 0.0f,
        50.0f, 0.0f,  50.0f,  100.0f, 100.0f,
        50.0f, 0.0f, -50.0f,  100.0f, 0.0f
    };

    m_layout = new VertexBufferLayout();
    m_VBO = new VertexBuffer(floorVertices, sizeof(floorVertices));

    m_layout->Push<float>(3);
    m_layout->Push<float>(2);

    m_VAO = new VertexArray();
    m_VAO->AddBuffer(*m_VBO, *m_layout);

    m_shader = new Shader("res/shaders/test.vert", "res/shaders/test.frag");
    m_texture = new Texture("res/textures/floor.jpg", T_SPECULAR);
}

Floor::~Floor()
{
    delete m_layout;
    delete m_VBO;
    delete m_VAO;
    delete m_shader;
    delete m_texture;
}

void Floor::Draw()
{
    glm::mat4 model = glm::mat4(1.0f);

    // Bind shader first before setting uniforms
    m_shader->Bind();
    
    // Set uniforms after shader is bound
    m_shader->SetUniform1i("uTexture", 1);
    m_shader->SetUniformMat4f("uModel", model);

    
    // Bind vertex data and texture
    m_VAO->Bind();
    m_texture->Bind(1);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

}