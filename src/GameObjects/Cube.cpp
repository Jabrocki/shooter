#include "Cube.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Camera.h"
#include "../Core/Window.h"

#include "../OpenGL/Debug.h"

Cube::Cube()
{
        // Cube
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    // Initialize the model matrix with the starting position
    m_Model = glm::translate(glm::mat4(1.0f), m_Position);



    m_layout = new VertexBufferLayout();
    m_VBO = new VertexBuffer(cubeVertices, sizeof(cubeVertices));

    m_layout->Push<float>(3);
    m_layout->Push<float>(2);

    m_VAO = new VertexArray();
    m_VAO->AddBuffer(*m_VBO, *m_layout);

    m_shader = new Shader("res/shaders/test.vert", "res/shaders/test.frag");
    m_texture = new Texture("res/textures/wall.jpg");
}

Cube::~Cube()
{
    delete m_layout;
    delete m_VBO;
    delete m_VAO;

    delete m_shader;
    delete m_texture;
}

void Cube::Update(float deltaTime)
{
    // Apply gravity to velocity
    m_Velocity.y += m_Gravity * deltaTime;
    
    // Update position based on velocity
    m_Position += m_Velocity * deltaTime;
    
    // Check for ground collision
    if (m_Position.y <= m_GroundLevel + 0.5f) // 0.5f is half the cube size
    {
        m_Position.y = m_GroundLevel + 0.5f;
        m_Velocity.y = 0.0f; // Stop vertical movement
    }
    
    // Update the model matrix with the new position
    m_Model = glm::translate(glm::mat4(1.0f), m_Position);
}

void Cube::Draw(glm::mat4 view, glm::mat4 projection)
{
    m_shader->Bind();

    // Set uniforms after shader is bound
    m_shader->SetUnifrom1i("uTexture", 0);
    m_shader->SetUniformMat4f("uModel", m_Model);
    m_shader->SetUniformMat4f("uView", view);
    m_shader->SetUniformMat4f("uProjection", projection);

    m_VAO->Bind();
    m_texture->Bind(0);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}