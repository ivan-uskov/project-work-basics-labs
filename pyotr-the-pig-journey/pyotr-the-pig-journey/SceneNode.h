#pragma once

#include "Category.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>
#include <set>
#include <memory>
#include <utility>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    explicit SceneNode(Category::Type category = Category::None);

    virtual void handleEvent(const sf::Event& event) {};
    void update(sf::Time dt, CommandQueue& commands);
    unsigned getCategory() const;

    template <typename T, typename... Args>
    void emplaceChild(Args&&... args)
    {
        attachChild(std::make_unique<T>(std::forward<Args>(args)...));
    }
    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode& node);
    void removeChildren();

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;
    virtual sf::FloatRect getBoundingRect() const;

    void onCommand(const Command& command, sf::Time dt);

    void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);

    void removeWrecks();
    virtual bool isMarkedForRemoval() const;

    void destroy();

protected:
    virtual void doDestroy() {};

    virtual void updateCurrent(sf::Time dt, CommandQueue& commands)                   {};
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {};

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

private:
    void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
    bool canCollide() const;

    void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    bool mMarkedForRemoval = false;
    std::vector<Ptr> mChildren;
    SceneNode * mParent = nullptr;
    Category::Type mCategory;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);