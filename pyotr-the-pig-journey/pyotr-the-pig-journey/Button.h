#pragma once

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "State.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

class SoundPlayer;

namespace GUI
{
    class Button : public Component
    {
    public:
        typedef std::shared_ptr<Button> Ptr;
        typedef std::function<void()> Callback;

        static const int BUTTON_WIDTH = 200;
        static const int BUTTON_HEIGHT = 50;
        static const int BUTTON_CHAR_SIZE = 20;

        enum Type
        {
            Normal,
            Selected,
            Pressed,
            ButtonCount
        };

    public:
        Button(State::Context context);

        void setCallback(Callback callback);
        void setText(const std::string& text);
        void setToggle(bool flag);

        bool isSelectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event& event) override;


    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        void changeTexture(Type buttonType);


    private:
        Callback mCallback;
        sf::Sprite mSprite;
        sf::Text mText;
        bool mIsToggle;
        SoundPlayer & mSounds;
    };
}