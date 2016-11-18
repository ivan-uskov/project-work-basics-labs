#include "stdafx.h"
#include "CubeNode.h"

CubeNode::CubeNode(const glm::vec3 & position, Cube && cube)
    : VisitableSceneNode<CubeNode>(position)
    , mCube(cube)
{}

const Cube & CubeNode::getCube() const
{
    return mCube;
}
