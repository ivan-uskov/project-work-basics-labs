#include "stdafx.h"
#include "CubeRenderer.h"

/*
        Y
        |
        |
        |
        +---X
       /
      /
     Z
        3----2
       /    /|
      /    / |
     7----6  |
     |  0 |  1
     |    | /
     |    |/
     4----5
*/

using namespace Math;

void CubeRenderer::render(const CubeNode & node) const
{
    draw(node, false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(node, true);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

std::vector<Vertex> CubeRenderer::fillVertices(const CubeNode & node) const
{
    auto position = node.getPosition();
    const auto x = position.x;
    const auto y = position.y;
    const auto z = position.z;
    const float hl = node.getCube().length() / 2.f;

    return{
        { { x - hl, y - hl, z - hl }, { 255, 0, 0, 255 } },       // 0
        { { x + hl, y - hl, z - hl }, { 255, 255, 0, 255 } },     // 1
        { { x + hl, y + hl, z - hl }, { 0, 255, 0, 255 } },       // 2
        { { x - hl, y + hl, z - hl }, { 0, 0, 0, 255 } },         // 3
        { { x - hl, y - hl, z + hl }, { 255, 0, 255, 255 } },     // 4
        { { x + hl, y - hl, z + hl }, { 255, 255, 255, 255 } },   // 5
        { { x + hl, y + hl, z + hl }, { 0, 255, 255, 255 } },     // 6
        { { x - hl, y + hl, z + hl }, { 0, 0, 255, 255 } }        // 7
    };
}

void CubeRenderer::draw(const CubeNode & node, bool isOnlyBorder) const
{
    auto vertices = fillVertices(node);

    for_each(vertices.begin(), vertices.end(), [&node](Math::Vertex & vertex) {
        node.prepareVertex(vertex);
    });

    //prepareVertexArray(vertices);

    if (isOnlyBorder)
    {
        for (auto & vert : vertices)
        {
            vert.pos.x *= 1.001f;
            vert.pos.y *= 1.001f;
            vert.pos.z *= 1.001f;
            vert.color.r = 0;
            vert.color.g = 0;
            vert.color.b = 0;
        }
    }

    const unsigned char faces[6][4] =
    {
        { 4, 7, 3, 0 },   // грань x < 0
        { 5, 1, 2, 6 },   // грань x > 0
        { 4, 0, 1, 5 },   // грань y < 0
        { 7, 6, 2, 3 },   // грань y > 0
        { 0, 3, 2, 1 },   // грань z < 0
        { 4, 5, 6, 7 },   // грань z > 0
    };

    glVertexPointer(VECTOR_3_SIZE, GL_FLOAT, sizeof(Vertex), &vertices[0].pos);

    glColorPointer(VECTOR_4_SIZE, GL_UNSIGNED_BYTE, sizeof(Vertex), &vertices[0].color);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
