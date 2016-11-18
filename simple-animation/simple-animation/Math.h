#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>

namespace Math
{
    static const int FULL_RING_D = 360;

    const int VECTOR_3_SIZE = 3;
    const int VECTOR_4_SIZE = 4;

    struct Vertex
    {
        glm::vec3 pos;
        glm::u8vec4 color;
    };
}

