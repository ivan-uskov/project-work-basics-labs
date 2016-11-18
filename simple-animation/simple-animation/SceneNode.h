#pragma once

#include "ISceneNode.h"
#include "Visitor.h"

class CubeNode;

template <typename T>
using VisitableSceneNode = VisitableWithBase<T, ISceneNode::SceneVisitor, SceneNode>;

class SceneNode : public ISceneNode
{
public:
    SceneNode(const glm::vec3 & position);

    void accept(SceneVisitor & visitor) const override;
    void update(sf::Time msec) override;

    virtual void addTransformation(std::unique_ptr<ITransformation> && transformation) override final;
    virtual void addChild(std::unique_ptr<ISceneNode> && child) override final;

    const glm::vec3 & getPosition() const;

    void prepareVertex(Math::Vertex & vertex) const;

protected:
    glm::vec3 mPosition;

    std::vector<std::unique_ptr<ITransformation>> mTransformations;
    std::vector<std::unique_ptr<ISceneNode>> mChildren;
};

