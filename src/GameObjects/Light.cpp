#include "Light.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Camera.h"
#include "../Core/Window.h"

#include "../OpenGL/Debug.h"

Light::Light()
{
    float lightVertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};

    

    m_layout = new VertexBufferLayout();
    m_VBO = new VertexBuffer(lightVertices, sizeof(lightVertices));

    m_layout->Push<float>(3);

    m_VAO = new VertexArray();
    m_VAO->AddBuffer(*m_VBO, *m_layout);

    m_shader = new Shader("res/shaders/light.vert", "res/shaders/light.frag");
}

Light::~Light()
{
    delete m_layout;
    delete m_VBO;
    delete m_VAO;
    delete m_shader;
}

void Light::Update(float deltaTime)
{
    m_Angle += m_AngularSpeed * deltaTime;

    if (m_Angle > glm::two_pi<float>())    // keep angle bounded
        m_Angle = std::fmod(m_Angle, glm::two_pi<float>());

    m_Position = glm::vec3(2*glm::sin(m_Angle), 1.0f, 2*glm::cos(m_Angle));
}

void Light::Draw()
{
    m_shader->Bind();

    // Set uniforms after shader is bound
    m_Model = glm::translate(glm::mat4(1.0f), m_Position);
    m_Model = glm::scale(m_Model, glm::vec3(0.2f));

    m_shader->SetUniformMat4f("uModel", m_Model);

    m_VAO->Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}