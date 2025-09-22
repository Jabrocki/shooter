#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    ~Transform() {};

    //Scale
    void SetScale(const glm::vec3& newScale);
    void MultScale(float mult);
    void DivScale(float div);

    void Translate(const glm::vec3& delta);

    void RotateEulerDegrees(const glm::vec3& eulerDegrees);


    glm::mat4 GetModelMatrix();
    glm::vec3 GetPosition();
private:
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
};
