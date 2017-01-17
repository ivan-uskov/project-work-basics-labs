#include "stdafx.h"
#include "StateStack.h"

#include <algorithm>
#include <cassert>

using namespace std;

StateStack::StateStack(State::Context context)
    : mContext(context)
{}

void StateStack::update(sf::Time dt)
{
    // Iterate from top to bottom, stop as soon as update() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update(dt))
        {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::draw()
{
    // Draw all active states from bottom to top
    std::for_each(mStack.begin(), mStack.end(), [](auto & state) {
        state->draw();
    });
}

void StateStack::handleEvent(const sf::Event& event)
{
    // Iterate from top to bottom, stop as soon as handleEvent() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(event))
        {
            break;
        }
    }

    applyPendingChanges();
}

States::ID StateStack::getCurrentState() const
{
    return mStack.empty() ? States::None : mStack.back()->getId();
}

void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

void StateStack::doInitialize()
{
    for (auto & state : mStates)
    {
        state.second->initialize();
    }
}

void StateStack::applyPendingChanges()
{
    for(auto change : mPendingList)
    {
        switch (change.action)
        {
            case Push:
            {
                auto & state = mStates[change.stateID];
                state->onActivate();
                mStack.push_back(state);
            } break;
            case Pop:
            {
                mStack.back()->onDeactivate();
                mStack.pop_back();

                if (!mStack.empty())
                {
                    mStack.back()->onActivate();
                }
            } break;
            case Clear:
            {
                for(auto & state : mStack)
                {
                    state->onDeactivate();
                }

                mStack.clear();
            }break;
        }
    }

    mPendingList.clear();
}