#include "stdafx.h"
#include "Player.h"

#include "CommandQueue.h"
#include "PlayerNode.h"

#include <map>
#include <string>
#include <algorithm>
#include <functional>

using namespace std::placeholders;

struct Mover
{
    Mover(float vx, float vy)
        : direction(vx, vy)
    {
    }

    void operator() (PlayerNode & tractor, sf::Time) const
    {
            tractor.go(direction);
    }

    sf::Vector2f direction;
};

Player::Player(sf::Int32 identifier, const KeyBinding* binding)
    : mKeyBinding(binding)
    , mCurrentMissionStatus(MissionRunning)
    , mIdentifier(identifier)
{
    initializeActions();

    // Assign all categories to player's tractor
    std::for_each(mActionBinding.begin(), mActionBinding.end(), [](auto & pair) {
        pair.second.category = Category::Tractor;
    });
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    // Event
    if (event.type == sf::Event::KeyPressed)
    {
        Action action;
        if (mKeyBinding && mKeyBinding->checkAction(event.key.code, action) && !isRealtimeAction(action))
        {
            commands.push(mActionBinding[action]);
        }
    }
}

void Player::handleRealtimeInput(CommandQueue & commands)
{
    // Lookup all actions and push corresponding commands to queue
    std::vector<Action> activeActions = mKeyBinding->getRealtimeActions();
    std::for_each(activeActions.begin(), activeActions.end(), [&](auto action) {
        commands.push(mActionBinding[action]);
    });
}

void Player::setMissionStatus(MissionStatus status)
{
    mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
    return mCurrentMissionStatus;
}

void Player::initializeActions()
{
    mActionBinding[PlayerAction::MoveLeft].action = derivedAction<PlayerNode>(Mover(-1, 0));
    mActionBinding[PlayerAction::MoveRight].action = derivedAction<PlayerNode>(Mover(+1, 0));
}
