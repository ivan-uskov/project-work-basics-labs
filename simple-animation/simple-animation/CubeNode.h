#pragma once

#include "Cube.h"
#include "SceneNode.h"

class CubeNode : public VisitableSceneNode<CubeNode>
{
public:
    CubeNode(const glm::vec3 & position, Cube && cube);

    const Cube & getCube() const;

private:
    Cube mCube;
};

