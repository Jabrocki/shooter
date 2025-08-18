#include "Game.h"

// GameObjects
#include "../GameObjects/Cube.h"
#include "../GameObjects/Light.h"

// OpenGL
#include "../OpenGL/Debug.h"
#include "../OpenGL/UniformBufferLayout.h"
#include "../OpenGL/UniformBuffer.h"

#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Game::Game() {}

Game::~Game() {}

void Game::Init()
{
    m_Window = std::make_unique<Window>(SCR_WIDTH, SCR_HEIGHT, "Game");

    // Mouse input
    m_Window->mouseMoveCallback = [this](double xpos, double ypos)
    {
        this->OnMouseMove(xpos, ypos);
    };

    // Capture mouse
    glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    GLCall(glEnable(GL_DEPTH_TEST));
}

void Game::Run()
{
    Shader cubeShader("res/shaders/cube.vert", "res/shaders/cube.frag");

    // Initialize projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // Create 10 cubes with example values using unique_ptr
    std::vector<std::unique_ptr<Cube>> cubes;

    // Cube positions, rotations (in radians), and scales
    glm::vec3 positions[10] = {
        glm::vec3(0.0f, 0.0f, -3.0f),
        glm::vec3(2.5f, 1.0f, -5.0f),
        glm::vec3(-3.0f, -1.5f, -4.0f),
        glm::vec3(1.0f, 3.0f, -6.0f),
        glm::vec3(-2.0f, 2.0f, -2.5f),
        glm::vec3(4.0f, -2.0f, -7.0f),
        glm::vec3(-1.5f, 0.5f, -3.5f),
        glm::vec3(3.5f, -1.0f, -4.5f),
        glm::vec3(-4.0f, 1.5f, -5.5f),
        glm::vec3(0.5f, -3.0f, -6.5f)};

    glm::vec3 rotations[10] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 1.2f, 0.3f),
        glm::vec3(1.0f, 0.8f, 2.1f),
        glm::vec3(0.7f, 2.5f, 1.4f),
        glm::vec3(1.8f, 0.4f, 0.9f),
        glm::vec3(2.2f, 1.7f, 0.6f),
        glm::vec3(0.3f, 3.0f, 1.1f),
        glm::vec3(1.5f, 0.2f, 2.8f),
        glm::vec3(2.7f, 1.9f, 0.1f),
        glm::vec3(0.9f, 2.3f, 1.6f)};

    glm::vec3 scales[10] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.8f, 1.2f, 0.9f),
        glm::vec3(1.5f, 0.7f, 1.1f),
        glm::vec3(0.6f, 0.6f, 0.6f),
        glm::vec3(1.3f, 1.3f, 1.3f),
        glm::vec3(0.9f, 1.8f, 0.5f),
        glm::vec3(1.7f, 0.4f, 1.6f),
        glm::vec3(0.5f, 1.0f, 2.0f),
        glm::vec3(1.1f, 0.8f, 0.7f),
        glm::vec3(1.4f, 1.6f, 0.9f)};

    for (int i = 0; i < 10; ++i)
    {
        cubes.push_back(std::make_unique<Cube>(positions[i], rotations[i], scales[i], &cubeShader));
    }

    Light light;
    UniformBufferLayout layout;

    layout.Push<glm::mat4>("uView");
    layout.Push<glm::mat4>("uProjection");
    layout.Push<glm::vec3>("uViewPosition");

    UniformBuffer UBO(layout);
    UBO.Bind();

    while (!m_Window->ShouldClose())
    {
        // Set Globals
        UBO.SetUniformMat4f("uView", m_Camera.GetViewMatrix());
        UBO.SetUniformMat4f("uProjection", projection);
        UBO.SetUniformVec3f("uViewPosition", m_Camera.GetPosition());

        float currentTime = glfwGetTime();
        m_DeltaTime = currentTime - m_LastFrame;
        m_LastFrame = currentTime;

        m_Window->PollEvents();
        ProcessInput();
        Update(m_DeltaTime);

        

        Clear();
        Draw();
        // Set light uniforms before drawing the cube
        cubeShader.Bind();

        cubeShader.SetUniformVec3f("uLight.position", light.getPosition());
        cubeShader.SetUniformVec3f("uLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        cubeShader.SetUniformVec3f("uLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        cubeShader.SetUniformVec3f("uLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        cubeShader.SetUniform1f("uLight.constant", 1.0f);
        cubeShader.SetUniform1f("uLight.linear", 0.09f);
        cubeShader.SetUniform1f("uLight.quadratic", 0.032f);

        // Draw all cubes
        for (auto &cube : cubes)
        {
            cube->Draw();
        }

        light.Draw();
        light.Update(m_DeltaTime);

        m_Window->SwapBuffers();
    }
}

void Game::ProcessInput()
{
    if (m_Window->IsKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(m_Window->GetNativeWindow(), true);

    glm::vec3 direction(0.0f);
    if (m_Window->IsKeyPressed(GLFW_KEY_W))
        direction += m_Camera.GetFront();
    if (m_Window->IsKeyPressed(GLFW_KEY_S))
        direction -= m_Camera.GetFront();
    if (m_Window->IsKeyPressed(GLFW_KEY_A))
        direction -= glm::normalize(glm::cross(m_Camera.GetFront(), glm::vec3(0, 1, 0)));
    if (m_Window->IsKeyPressed(GLFW_KEY_D))
        direction += glm::normalize(glm::cross(m_Camera.GetFront(), glm::vec3(0, 1, 0)));

    if (glm::length(direction) > 0.0f)
        m_Camera.ProcessKeyboardMovement(glm::normalize(direction), m_DeltaTime);
}

void Game::OnMouseMove(double xpos, double ypos)
{
    if (m_FirstMouse)
    {
        m_LastX = xpos;
        m_LastY = ypos;
        m_FirstMouse = false;
    }

    float xoffset = xpos - m_LastX;
    float yoffset = m_LastY - ypos; // reversed: y goes from bottom to top

    m_LastX = xpos;
    m_LastY = ypos;

    m_Camera.ProcessMouseMovement(xoffset, yoffset);
}

void Game::Update(float deltaTime)
{
    // Future: game object updates, physics, logic
}

void Game::Clear()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Game::Draw()
{
    // Future: pass camera view/projection to renderer
}
