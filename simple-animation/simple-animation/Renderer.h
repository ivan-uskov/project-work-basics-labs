#pragma once

#include "ISceneNode.h"
#include "CubeRenderer.h"
#include "SphereRenderer.h"

class Renderer : public ISceneNode::SceneVisitor
{
public:
    void render(const ISceneNode & node);

    void visit(const SphereNode & sphereNode) override;
    void visit(const CubeNode & cubeNode) override;
    void visit(const SceneNode & sceneNode) override {}

private:
    std::unique_ptr<CubeRenderer> mCubeRenderer;
    std::unique_ptr<SphereRenderer> mSphereRenderer;
};

