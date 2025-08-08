#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    ~Transform() {};

    //Rotation
    void Translate(const glm::vec3& delta);
    void SetScale(const glm::vec3& newScale);
    void RotateEulerDegrees(const glm::vec3& eulerDegrees);


    glm::mat4 GetModelMatrix();
private:
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
};
