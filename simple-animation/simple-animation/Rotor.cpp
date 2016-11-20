#include "stdafx.h"
#include "Rotor.h"

using namespace Math;
using namespace glm;

namespace
{
    const float SPEED_COEF = 0.001f;
}

Rotor::Rotor(int rotatingSpeed, glm::vec3 const& rotationCenter, glm::vec3 const& rotationAxis)
    : m_rotationSpeed(rotatingSpeed)
    , m_rotationCenter(rotationCenter)
    , m_rotationAxis(normalize(rotationAxis))
{
    m_currentAngle = float(rand() % Math::FULL_RING_D);
}

void Rotor::update(sf::Time msec)
{
    m_currentAngle += float(abs(cos(msec.asMilliseconds() / 4))) * float(m_rotationSpeed) * SPEED_COEF;
    if (m_currentAngle > FULL_RING_D)
    {
        m_currentAngle -= floor(m_currentAngle / FULL_RING_D) * FULL_RING_D;
    }

    prepareRotationMatrix();
}

void Rotor::accept(Vertex & vertex) const
{
    vertex.pos = rotate(vertex.pos - m_rotationCenter, radians(m_currentAngle), m_rotationAxis) + m_rotationCenter;
}

void Rotor::prepareRotationMatrix()
{
    auto mtx = translate(-m_rotationCenter);
    mtx = rotate(mtx, radians(m_currentAngle), m_rotationAxis);
    m_rotationMtx = translate(mtx, m_rotationCenter);
}
