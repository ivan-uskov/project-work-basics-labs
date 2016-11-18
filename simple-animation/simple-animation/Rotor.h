#pragma once

#include "ITransformation.h"

class Rotor : public ITransformation
{
public:
    Rotor(int rotatingSpeed, glm::vec3 const& rotationCenter, glm::vec3 const& rotationAxis);

    virtual void update(sf::Time msec) override;
    virtual void accept(Math::Vertex & vertex) const override;

private:

    void prepareRotationMatrix();

    int m_rotationSpeed;
    float m_currentAngle = 0;

    glm::vec3 m_rotationCenter;
    glm::vec3 m_rotationAxis;
    glm::mat4x4 m_rotationMtx;
};

