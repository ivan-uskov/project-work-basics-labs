#include "stdafx.h"
#include "State.h"

#include "StateStack.h"

State::Context::Context(
    sf::RenderWindow & window,
    TextureHolder & textures,
    FontHolder & fonts,
    MusicPlayer & music,
    SoundPlayer & sounds,
    KeyBinding & keys1,
    KeyBinding & keys2
)
    : window(window)
    , textures(textures)
    , fonts(fonts)
    , music(music)
    , sounds(sounds)
    , keys1(keys1)
    , keys2(keys2)
{}

State::State(States::ID stateId, StateStack & stack, Context context)
    : mStack(&stack)
    , mContext(context)
    , mStateId(stateId)
{}

void State::requestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStack->popState();
}

void State::requestStateClear()
{
    mStack->clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}

States::ID State::getId() const
{
    return mStateId;
}