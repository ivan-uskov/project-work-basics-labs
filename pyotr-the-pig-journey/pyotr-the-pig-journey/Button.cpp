#include "stdafx.h"
#include "Button.h"

#include "Utility.h"
#include "SoundPlayer.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
    Button::Button(State::Context context)
        : Component(true)
        , mSprite(context.textures[Textures::Buttons])
        , mText("", context.fonts[Fonts::Main], BUTTON_CHAR_SIZE)
        , mIsToggle(false)
        , mSounds(context.sounds)
    {
        changeTexture(Normal);

        sf::FloatRect bounds = mSprite.getLocalBounds();
        mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
        mText.setFillColor(sf::Color::Black);
    }

    void Button::setCallback(Callback callback)
    {
        mCallback = std::move(callback);
    }

    void Button::setText(const std::string& text)
    {
        mText.setString(text);
        centerOrigin(mText);
    }

    void Button::setToggle(bool flag)
    {
        mIsToggle = flag;
    }

    void Button::select()
    {
        Component::select();

        changeTexture(Selected);
    }

    void Button::deselect()
    {
        Component::deselect();

        changeTexture(Normal);
    }

    void Button::activate()
    {
        Component::activate();

        // If we are toggle then we should show that the button is pressed and this "toggled".
        if (mIsToggle)
        {
            changeTexture(Pressed);
        }

        if (mCallback)
        {
            mCallback();
        }

        // If we are not a toggle then deactivate the button since we are just momentarily activated.
        if (!mIsToggle)
        {
            deactivate();
        }

        mSounds.play(SoundEffect::Button);
    }

    void Button::deactivate()
    {
        Component::deactivate();

        if (mIsToggle)
        {
            // Reset texture to right one depending on if we are selected or not.
            if (isSelected())
            {
                changeTexture(Selected);
            }
            else
            {
                changeTexture(Normal);
            }
        }
    }

    void Button::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mSprite, states);
        target.draw(mText, states);
    }

    void Button::changeTexture(Type buttonType)
    {
        sf::IntRect textureRect(0, BUTTON_HEIGHT * buttonType, BUTTON_WIDTH, BUTTON_HEIGHT);
        mSprite.setTextureRect(textureRect);
    }

    sf::FloatRect Button::getBoundingRect() const
    {
        return getWorldTransform().transformRect(mSprite.getGlobalBounds());
    }
}
