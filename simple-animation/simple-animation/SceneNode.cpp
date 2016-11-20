#include "stdafx.h"
#include "SceneNode.h"

using namespace std;

SceneNode::SceneNode(const glm::vec3 & position)
    : mPosition(position)
{}

void SceneNode::accept(SceneVisitor & visitor) const
{
    std::for_each(mChildren.begin(), mChildren.end(), [&visitor](auto & childPtr) {
        childPtr->accept(visitor);
    });
}

void SceneNode::addTransformation(std::unique_ptr<ITransformation> && transformation)
{
    mTransformations.push_back(move(transformation));
}

void SceneNode::addChild(std::unique_ptr<ISceneNode> && child)
{
    mChildren.push_back(move(child));
}

void SceneNode::update(sf::Time msec)
{
    for (auto & ts : mTransformations)
    {
        ts->update(msec);
    };

    for (auto & childPtr : mChildren)
    {
        childPtr->update(msec);
    };
}

const glm::vec3 & SceneNode::getPosition() const
{
    return mPosition;
}

void SceneNode::prepareVertex(Math::Vertex & vertex) const
{
    for_each(mTransformations.begin(), mTransformations.end(), [&vertex](auto const& transformation) {
        transformation->accept(vertex);
    });
}