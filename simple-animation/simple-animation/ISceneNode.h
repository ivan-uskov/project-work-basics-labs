#pragma once

#include <memory>
#include <SFML/System/Time.hpp>

#include "ITransformation.h"
#include "Visitor.h"

class CubeNode;
class SceneNode;
class SphereNode;

class ISceneNode
{
public:
    typedef Visitor<SphereNode, CubeNode, SceneNode> SceneVisitor;

    virtual ~ISceneNode() = default;
    virtual void update(sf::Time msec) = 0;

    virtual void accept(SceneVisitor & visitor) const = 0;

    virtual void addTransformation(std::unique_ptr<ITransformation> && transformation) = 0;
    virtual void addChild(std::unique_ptr<ISceneNode> && child) = 0;
};