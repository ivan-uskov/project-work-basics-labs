#include "stdafx.h"
#include "Cube.h"

Cube::Cube(glm::vec3 const& position, int length)
    : m_position(position)
    , m_length(length)
{
}

glm::vec3 Cube::position() const
{
    return m_position;
}

int Cube::length() const
{
    return m_length;
}
