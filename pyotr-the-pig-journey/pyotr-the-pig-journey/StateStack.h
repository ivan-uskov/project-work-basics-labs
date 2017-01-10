#pragma once

#include "State.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

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

class StateStack : private sf::NonCopyable
{
public:
    explicit StateStack(State::Context context);

    template <typename StateType, typename ...Parameters>
    void registerState(States::ID stateID, Parameters&&... params)
    {
        mFactories[stateID] = [&] {
            return std::make_unique<StateType>(*this, mContext, std::forward<Parameters>(params)...);
        };
    }

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;
    void applyPendingChanges();

private:
    State::Ptr createState(States::ID stateID);

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
    std::map<States::ID, std::function<State::Ptr()>> mFactories;
};