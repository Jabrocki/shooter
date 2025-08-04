#pragma once

#include <memory>
#include "Window.h"
#include "Camera.h"

class Game
{
public:
    Game();
    ~Game();

    void Init();
    void Run();

private:
    std::unique_ptr<Window> m_Window;
    Camera m_Camera;

    // Timing
    float m_LastFrame = 0.0f;
    float m_DeltaTime = 0.0f;

    // Mouse state
    double m_LastX = 400.0;  // screen center
    double m_LastY = 300.0;
    bool m_FirstMouse = true;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Input + callbacks
    void ProcessInput();
    void OnMouseMove(double xpos, double ypos);

    // Game loop phases
    void Update(float deltaTime);
    void Clear();
    void Draw();
};
