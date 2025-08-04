#include "Camera.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch)
    : m_Position(position),
      m_Yaw(yaw),
      m_Pitch(pitch),
      m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    m_Yaw   += xOffset;
    m_Pitch += yOffset;

    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessKeyboardMovement(glm::vec3 direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    m_Position += direction * velocity;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}
