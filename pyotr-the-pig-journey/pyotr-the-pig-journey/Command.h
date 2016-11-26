#pragma once

#include "Category.h"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
    typedef std::function<void(SceneNode&, sf::Time)> Action;

    Command() = default;
    Command(unsigned category, Action && action);

    Action action;
    unsigned int category = Category::None;
};

template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn)
{
    return [=](SceneNode& node, sf::Time dt) {
        // Check if cast is safe
        assert(dynamic_cast<GameObject*>(&node) != nullptr);

        fn(static_cast<GameObject&>(node), dt);
    };
}
