#pragma once

#define GLM_FORCE_RADIANS

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Math
{
    typedef unsigned int VertexIndex;

    struct Triangle
    {
        VertexIndex p1, p2, p3;
    };

    static const int FULL_RING_D = 360;
    static const int HALF_RING_D = 180;

    const int VECTOR_3_SIZE = 3;
    const int VECTOR_4_SIZE = 4;

    const auto X_AXIS = glm::vec3(1, 0, 0);
    const auto Y_AXIS = glm::vec3(0, 1, 0);
    const auto Z_AXIS = glm::vec3(0, 0, 1);

    struct Vertex
    {
        glm::vec3 pos;
        glm::u8vec4 color;
    };

    struct GLPosition
    {
        float x, y, z;
    };

    struct GLColor
    {
        uint8_t r, g, b, a;
    };

    struct GLVertex
    {
        GLPosition pos;
        GLColor color;
    };

    glm::vec3 middle(glm::vec3 const& p1, glm::vec3 const& p2);
    std::vector<VertexIndex> triangleToVertexIndexArray(std::vector<Triangle> const& src);
    std::vector<Vertex> vertexPositionsToVertexArray(std::vector<glm::vec3> const& src);

}

