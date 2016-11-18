#pragma once

#include "CubeNode.h"

class CubeRenderer
{
public:
    void render(const CubeNode & node) const;

private:
    std::vector<Math::Vertex> fillVertices(const CubeNode & node) const;
    void draw(const CubeNode & node, bool isOnlyBorder) const;
};

