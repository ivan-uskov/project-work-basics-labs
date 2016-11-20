#include "stdafx.h"
#include "Renderer.h"

using namespace std;

void Renderer::render(const ISceneNode & node)
{
    node.accept(*this);
}

void Renderer::visit(const CubeNode & cubeNode)
{
    if (!mCubeRenderer)
    {
        mCubeRenderer = make_unique<CubeRenderer>();
    }

    mCubeRenderer->render(cubeNode);
}

void Renderer::visit(const SphereNode & sphereNode)
{
    if (!mSphereRenderer)
    {
        mSphereRenderer = make_unique<SphereRenderer>();
    }

    mSphereRenderer->render(sphereNode);
}