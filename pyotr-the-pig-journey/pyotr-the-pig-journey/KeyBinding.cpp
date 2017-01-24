#include "stdafx.h"
#include "KeyBinding.h"

#include <string>
#include <algorithm>


KeyBinding::KeyBinding(int controlPreconfiguration)
{
    // Set initial key bindings for player 1
    if (controlPreconfiguration == 1)
    {
        mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveLeft;
        mKeyMap[sf::Keyboard::Right] = PlayerAction::MoveRight;
        mKeyMap[sf::Keyboard::Up] = PlayerAction::MoveUp;
        mKeyMap[sf::Keyboard::Down] = PlayerAction::MoveDown;
        mKeyMap[sf::Keyboard::Space] = PlayerAction::Fire;
    }
    else if (controlPreconfiguration == 2)
    {
        // Player 2
        mKeyMap[sf::Keyboard::A] = PlayerAction::MoveLeft;
        mKeyMap[sf::Keyboard::D] = PlayerAction::MoveRight;
        mKeyMap[sf::Keyboard::W] = PlayerAction::MoveUp;
        mKeyMap[sf::Keyboard::S] = PlayerAction::MoveDown;
        mKeyMap[sf::Keyboard::F] = PlayerAction::Fire;
    }
}

void KeyBinding::assignKey(Action action, sf::Keyboard::Key key)
{
    for (auto itr = mKeyMap.begin(); itr != mKeyMap.end(); )
    {
        if (itr->second == action)
        {
            mKeyMap.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }

    mKeyMap[key] = action;
}

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
    auto it = std::find_if(mKeyMap.begin(), mKeyMap.end(), [&action](auto const& pair) {
        return pair.second == action;
    });

    return (it != mKeyMap.end()) ? it->first : sf::Keyboard::Unknown;
}

bool KeyBinding::checkAction(sf::Keyboard::Key key, Action& out) const
{
    auto found = mKeyMap.find(key);
    if (found == mKeyMap.end())
    {
        return false;
    }
    else
    {
        out = found->second;
        return true;
    }
}

std::vector<KeyBinding::Action> KeyBinding::getRealtimeActions() const
{
    // Return all realtime actions that are currently active.
    std::vector<Action> actions;

    auto it = std::for_each(mKeyMap.begin(), mKeyMap.end(), [&actions](auto const& pair) {
        if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        {
            actions.push_back(pair.second);
        }
    });

    return actions;
}

bool isRealtimeAction(PlayerAction::Type action)
{
    switch (action)
    {
    case PlayerAction::MoveLeft:
    case PlayerAction::MoveRight:
    case PlayerAction::MoveDown:
    case PlayerAction::MoveUp:
    case PlayerAction::Fire:
        return true;

    default:
        return false;
    }
}