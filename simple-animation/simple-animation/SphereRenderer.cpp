#include "stdafx.h"
#include "SphereRenderer.h"
#include "Tetrahedron.h"

using namespace std;
using namespace Math;
using namespace glm;

namespace
{
    auto fillNormales(const vec3 & position, vector<Vertex> const& vertices)
    {
        vector<vec3> normales;
        normales.reserve(vertices.size());

        for (auto const& vertex : vertices)
        {
            normales.push_back(normalize((vertex.pos - position)));
        }

        return normales;
    }
}

void SphereRenderer::render(const SphereNode & node)
{
    if (needUpdateCache(node))
    {
        updateCache(node);
    }

    draw(node, false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(node, true);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SphereRenderer::draw(const SphereNode & node, bool isOnlyBorder)
{
    auto vertices = mCache->vertices;
    auto centerVertex = Vertex{ node.getPosition() };
    node.prepareVertex(centerVertex);

    for (auto & vertex : vertices)
    {
        node.prepareVertex(vertex);
    };

    if (isOnlyBorder)
    {
        for (auto & vert : vertices)
        {
            vert.pos.x *= 1.0001f;
            vert.pos.y *= 1.0001f;
            vert.pos.z *= 1.0001f;
            vert.color.r = 0;
            vert.color.g = 0;
            vert.color.b = 0;
        }
    }

    glBegin(GL_TRIANGLE_STRIP);
    int l = vertices.size();
    for (auto const& vertex : vertices)
    {
        glColor4ubv(&vertex.color.r);
        glVertex3fv(&vertex.pos.x);
    }
    glEnd();
}

bool SphereRenderer::needUpdateCache(const SphereNode & node) const
{
    return !mCache || (mCache->node.getPosition() != node.getPosition()) || (mCache->node.getSphere() != node.getSphere());
}

void SphereRenderer::updateCache(const SphereNode & node)
{
    mCache = unique_ptr<Cache>(new Cache{
        SphereNode(node.getPosition(), Sphere(node.getSphere())),
        vertexPositionsToVertexArray(triangulate(node))
    });
}

vector<vec3> SphereRenderer::triangulate(const SphereNode & node) const
{
    auto a = node.getPosition().x;
    auto b = node.getPosition().y;
    auto c = node.getPosition().z;
    auto r = node.getSphere().radius();

    vector<vec3> vertices;

    auto angleDeg = 10.f;
    auto angle = glm::radians(angleDeg);
    int p2 = 360 / int(angleDeg);
    int r2 = 180 / int(angleDeg);
    for (auto y = -r2; y < r2; ++y)
    {
        auto cy = float(cos(y * angle));
        auto cy1 = float(cos((y + 1) * angle));
        auto sy = float(sin(y * angle));
        auto sy1 = float(sin((y + 1) * angle));

        for (auto i = -p2; i < p2; ++i)
        {
            auto ci = float(cos(i * angle));
            auto si = float(sin(i * angle));
            vertices.push_back(vec3(a + r * ci * cy, b + r * sy, c + r * si * cy));
            vertices.push_back(vec3(a + r * ci * cy1, b + r * sy1, c + r * si * cy1));
        }
    }

    return vertices;
}