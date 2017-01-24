#pragma once

#include "Component.h"
#include <SFML/Window/Event.hpp>

#include <vector>
#include <memory>

namespace GUI
{
    class Container : public Component
    {
    public:
        typedef std::shared_ptr<Container> Ptr;

    public:
        Container() = default;

        void pack(Component::Ptr component);

        virtual void handleEvent(const sf::Event& event);

    protected:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        bool hasSelection() const;
        void select(std::size_t index);
        void selectNext();
        void selectPrevious();

        void handleKeyReleased(int key);
        void handleMouseMoved(const sf::Event::MouseMoveEvent & position);
        void handleMouseClicked(const sf::Event::MouseButtonEvent & event);

    private:
        std::vector<Component::Ptr> mChildren;
        int mSelectedChild = -1;
    };

}