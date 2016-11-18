#include "stdafx.h"
#include "Rotor.h"

using namespace Math;

#include <glm/trigonometric.hpp>

namespace
{
    const float SPEED_COEF = 0.001f;
}

Rotor::Rotor(int rotatingSpeed, glm::vec3 const& rotationCenter, glm::vec3 const& rotationAxis)
    : m_rotationSpeed(rotatingSpeed)
    , m_rotationCenter(rotationCenter)
    , m_rotationAxis(rotationAxis)
    , m_currentAngle(rand() % Math::FULL_RING_D)
{}

void Rotor::update(sf::Time msec)
{
    m_currentAngle += msec.asMilliseconds() * m_rotationSpeed * SPEED_COEF;
    if (m_currentAngle > FULL_RING_D)
    {
        m_currentAngle -= floor(m_currentAngle / FULL_RING_D) * FULL_RING_D;
    }

    prepareRotationMatrix();
}

void Rotor::accept(Vertex & vertex) const
{
    vertex.pos = m_rotationMtx * glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 0.f);;
}

void Rotor::prepareRotationMatrix()
{
    auto mtx = glm::translate(m_rotationCenter);
    mtx = glm::rotate(mtx, m_currentAngle, m_rotationAxis);
    m_rotationMtx = glm::translate(mtx, -m_rotationCenter);
}
