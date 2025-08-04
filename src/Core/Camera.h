#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(
        glm::vec3 position = {0.0f, 1.0f, 1.0f},
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    glm::mat4 GetViewMatrix() const;
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void ProcessKeyboardMovement(glm::vec3 direction, float deltaTime);

    // Accessors
    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetFront() const { return m_Front; }

    float MovementSpeed = 2.5f;
    float MouseSensitivity = 0.1f;

private:
    void UpdateCameraVectors();

    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;
};
