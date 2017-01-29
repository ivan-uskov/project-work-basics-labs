#include "stdafx.h"
#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace std;

namespace GUI
{
    void Container::pack(Component::Ptr component)
    {
        mChildren.push_back(component);

        if (!hasSelection() && component->isSelectable())
        {
            select(mChildren.size() - 1);
        }
    }

    void Container::handleEvent(const sf::Event& event)
    {
        // If we have selected a child then give it events
        if (hasSelection() && mChildren[mSelectedChild]->isActive())
        {
            mChildren[mSelectedChild]->handleEvent(event);
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            handleKeyReleased(event.key.code);
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            handleMouseMoved(event.mouseMove);
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            handleMouseClicked(event.mouseButton);
        }
    }

    void Container::handleMouseMoved(const sf::Event::MouseMoveEvent & event)
    {
        auto it = find_if(mChildren.begin(), mChildren.end(), [&event](const Component::Ptr & child) {
            return child->getBoundingRect().contains(float(event.x), float(event.y));
        });

        if (it != mChildren.end())
        {
            for (auto & child : mChildren)
            {
                child->deselect();
            }

            (*it)->select();
        }
    }

    void Container::handleMouseClicked(const sf::Event::MouseButtonEvent & event)
    {
        if (event.button == sf::Mouse::Left)
        {
            auto it = find_if(mChildren.begin(), mChildren.end(), [&event](const Component::Ptr & child) {
                return child->getBoundingRect().contains(float(event.x), float(event.y));
            });

            if (it != mChildren.end())
            {
                for (auto & child : mChildren)
                {
                    child->deactivate();
                }

                (*it)->activate();
            }
        }
    }

    void Container::handleKeyReleased(int key)
    {
        if (
            key == sf::Keyboard::W ||
            key == sf::Keyboard::Up ||
            key == sf::Keyboard::Left
            )
        {
            selectPrevious();
        }
        else if (
            key == sf::Keyboard::S ||
            key == sf::Keyboard::Down ||
            key == sf::Keyboard::Right
            )
        {
            selectNext();
        }
        else if (key == sf::Keyboard::Return || key == sf::Keyboard::Space)
        {
            if (hasSelection())
            {
                mChildren[mSelectedChild]->activate();
            }
        }
    }

    void Container::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (const Component::Ptr& child : mChildren)
        {
            target.draw(*child, states);
        }
    }

    bool Container::hasSelection() const
    {
        return mSelectedChild >= 0;
    }

    void Container::select(std::size_t index)
    {
        if (mChildren[index]->isSelectable())
        {
            if (hasSelection())
            {
                mChildren[mSelectedChild]->deselect();
            }

            mChildren[index]->select();
            mSelectedChild = int(index);
        }
    }

    void Container::selectNext()
    {
        if (!hasSelection())
        {
            return;
        }

        // Search next component that is selectable, wrap around if necessary
        int next = mSelectedChild;
        do
        {
            next = (next + 1) % mChildren.size();
        }
        while (!mChildren[next]->isSelectable());

        // Select that component
        select(next);
    }

    void Container::selectPrevious()
    {
        if (!hasSelection())
        {
            return;
        }

        // Search previous component that is selectable, wrap around if necessary
        int prev = mSelectedChild;
        do
        {
            prev = int((prev + mChildren.size() - 1) % mChildren.size());
        }
        while (!mChildren[prev]->isSelectable());

        select(prev);
    }
}
