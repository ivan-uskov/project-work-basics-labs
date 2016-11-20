#pragma once

#include "SphereNode.h"
#include "Math.h"

class SphereRenderer
{
public:
    void render(const SphereNode & node);

private:
    void draw(const SphereNode & node, bool isOnlyBorder = false);

    std::vector<glm::vec3> triangulate(const SphereNode & node) const;

    bool needUpdateCache(const SphereNode & node) const;
    void updateCache(const SphereNode & node);

private:
    struct Cache
    {
        SphereNode node;
        std::vector<Math::Vertex> vertices;
    };

    std::unique_ptr<Cache> mCache;

};

