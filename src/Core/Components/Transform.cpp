#include "Transform.h"

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    m_Position = position;
    m_Rotation = glm::quat(rotation);
    m_Scale = scale;
}

glm::mat4 Transform::GetModelMatrix()
{
    glm::mat4 T = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 R = glm::mat4_cast(m_Rotation);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), m_Scale);
    return T * R * S;
}

void Transform::MultScale(float mult)
{
    m_Scale *= mult;
}

void Transform::DivScale(float div)
{
    m_Scale /= div;
}

glm::vec3 Transform::GetPosition()
{
    return m_Position;
}