#pragma once

#include "LazyLoad.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
    class RenderWindow;
}

class StateStack;
class MusicPlayer;
class SoundPlayer;
class KeyBinding;

class State : public LazyLoad
{
public:
    typedef std::shared_ptr<State> Ptr;

    struct Context
    {
        Context(
            sf::RenderWindow & window,
            TextureHolder & textures,
            FontHolder & fonts,
            MusicPlayer & music,
            SoundPlayer & sounds,
            KeyBinding & keys1,
            KeyBinding & keys2
        );

        sf::RenderWindow * window;
        TextureHolder * textures;
        FontHolder * fonts;
        MusicPlayer * music;
        SoundPlayer * sounds;
        KeyBinding * keys1;
        KeyBinding * keys2;
    };

public:
    State(States::ID stateId, StateStack & stack, Context context);
    virtual ~State() = default;

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

    virtual void onActivate()
    {
        initialize();
    };

    States::ID getId() const;

    virtual void onDeactivate() {};

protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;

    template <typename T>
    T & getState(States::ID stateId)
    {
        return static_cast<T&>(*(mStack->getState(stateId)));
    }

private:
    StateStack * mStack;
    Context mContext;
    States::ID mStateId;
};
