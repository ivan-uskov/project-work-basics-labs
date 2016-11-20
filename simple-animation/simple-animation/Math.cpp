#include "stdafx.h"
#include "Math.h"

namespace Math
{
    glm::vec3 middle(glm::vec3 const& p1, glm::vec3 const& p2)
    {
        return (p1 + p2) / 2.f;
    }

    std::vector<VertexIndex> triangleToVertexIndexArray(std::vector<Triangle> const& src)
    {
        std::vector<VertexIndex> dst;
        dst.reserve(src.size());

        for (auto const& tr : src)
        {
            dst.push_back(tr.p1);
            dst.push_back(tr.p2);
            dst.push_back(tr.p3);
        }

        return dst;
    }

    std::vector<Vertex> vertexPositionsToVertexArray(std::vector<glm::vec3> const& src)
    {
        std::vector<Vertex> dst;
        dst.reserve(src.size());

        for (auto const& vertex : src)
        {
            dst.push_back({
                { vertex.x, vertex.y, vertex.z },
                { 255, 255, 255, 255 }
            });
        }

        return dst;
    }
}

/*

struct GLPosition
{
    float x, y, z;
};

struct GLColor
{
    float r, g, b, a;
};

struct GLVertex
{
    GLPosition pos;
    GLColor color;
};

struct Vertex
{
    glm::vec3 pos;
    glm::u8vec4 color;
};

template <typename Src, typename Dst, template <typename...> class Container = std::vector>
auto array_map(const Container<Src> & values, std::function<typename Dst(typename Src const&)> && func)
{
    Container<Dst> result(values.size());

    for (auto const& value : values)
    {
        result.push_back(func(value));
    }

    return result;
}

auto toGLArray(const std::vector<Vertex> & vertices)
{
    return array_map(vertices, [](Vertex const& vertex) mutable -> GLVertex {
        return {
            { vertex.pos.x, vertex.pos.y, vertex.pos.z },
            { vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a }
        };
    });
}

*/