#pragma once

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
    class Label : public Component
    {
    public:
        typedef std::shared_ptr<Label> Ptr;

        Label(const std::string & text, const FontHolder& fonts);

        void setText(const std::string& text);

        sf::FloatRect getBoundingRect() const override;

    protected:
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        sf::Text mText;
    };
}