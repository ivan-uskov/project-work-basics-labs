#include "stdafx.h"
#include "Label.h"

#include "Utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

    Label::Label(const std::string& text, const FontHolder& fonts)
        : mText(text, fonts[Fonts::Main], 16)
    {
        mText.setFillColor(sf::Color::Black);
    }

    void Label::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mText, states);
    }

    void Label::setText(const std::string& text)
    {
        mText.setString(text);
    }

    sf::FloatRect Label::getBoundingRect() const
    {
        return getWorldTransform().transformRect(mText.getGlobalBounds());
    }
}