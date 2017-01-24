#include "stdafx.h"
#include "SceneNode.h"

#include "Command.h"
#include "Utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

SceneNode::SceneNode(Category::Type category)
    : mCategory(category)
{}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
    assert(found != mChildren.end());

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);

    for (auto & child : mChildren)
    {
        child->update(dt, commands);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    drawCurrent(target, states);

    for (auto const& child : mChildren)
    {
        child->draw(target, states);
    }

    // Draw bounding rectangle - disabled by default
    //drawBoundingRect(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
    auto rect = getBoundingRect();
    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
    auto transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr; node = node->mParent)
    {
        transform = node->getTransform() * transform;
    }

    return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory())
    {
        command.action(*this, dt);
    }

    for (auto & child : mChildren)
    {
        child->onCommand(command, dt);
    }
}

unsigned SceneNode::getCategory() const
{
    return mCategory;
}

bool SceneNode::canCollide() const
{
    return (mCategory & Category::CanCollide) && !SceneNode::isMarkedForRemoval();
}

void SceneNode::checkSceneCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if (node.canCollide())
    {
        checkNodeCollision(node, collisionPairs);
    }

    for (auto const& nodeChild : node.mChildren)
    {
        checkSceneCollision(*nodeChild, collisionPairs);
    }
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if (this != std::addressof(node) && canCollide() && node.canCollide() && collision(*this, node))
    {
        collisionPairs.insert(std::minmax(this, &node));
    }

    for (auto & child : mChildren)
    {
        child->checkNodeCollision(node, collisionPairs);
    }
}

void SceneNode::removeChildren()
{
    mChildren.clear();
}

void SceneNode::removeWrecks()
{
    // Remove all children which request so
    auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    mChildren.erase(wreckfieldBegin, mChildren.end());

    // Call function recursively for all remaining children
    std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    auto bounds = std::shared_ptr<sf::FloatRect>();
    for (auto & child : mChildren)
    {
        auto childRect = child->getBoundingRect();
        if (bounds)
        {
            *bounds = max(*bounds, childRect);
        }
        else
        {
            bounds = std::make_shared<sf::FloatRect>(childRect);
        }
    }

    return bounds ? *bounds : sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
    return mMarkedForRemoval;
}

void SceneNode::destroy()
{
    mMarkedForRemoval = true;
    doDestroy();
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
