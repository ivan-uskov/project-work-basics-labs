#pragma once

#include "Math.h"

class Cube
{
public:
    Cube(glm::vec3 const& position, int length);

    glm::vec3 position() const;
    int length() const;

private:
    glm::vec3 m_position;
    int m_length;
};
