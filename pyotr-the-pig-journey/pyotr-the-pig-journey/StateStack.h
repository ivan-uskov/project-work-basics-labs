#pragma once

#include "State.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"
#include "LazyLoad.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack : private sf::NonCopyable, public LazyLoad
{
public:
    explicit StateStack(State::Context context);

    template <typename StateType, typename ...Parameters>
    void registerState(States::ID stateID, Parameters &&... params)
    {
        mStates[stateID] = std::make_shared<StateType>(stateID, *this, mContext, std::forward<Parameters>(params)...);
    }

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    States::ID getCurrentState() const;
    State::Ptr getState(States::ID stateId);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;
    void applyPendingChanges();

protected:
    void doInitialize() override;

private:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };

    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID stateID = States::None)
            : action(action), stateID(stateID) {}

        Action action;
        States::ID stateID;
    };

private:
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;

    State::Context mContext;
    std::map<States::ID, State::Ptr> mStates;
};