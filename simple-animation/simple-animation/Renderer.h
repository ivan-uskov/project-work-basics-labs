#pragma once

#include "ISceneNode.h"
#include "CubeRenderer.h"

class Renderer : public ISceneNode::SceneVisitor
{
public:
    void render(const ISceneNode & node);

    void visit(const CubeNode & cubeNode) override;
    void visit(const SceneNode & cubeNode) override {}

private:
    std::unique_ptr<CubeRenderer> mCubeRenderer;
};

