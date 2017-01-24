#pragma once

#include "SceneNode.h"

namespace sf
{
    class Event;
}

namespace GUI
{
    class Component : public SceneNode
    {
    public:
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component(bool isSelectable = false);

        bool isSelected() const;
        virtual void select();
        virtual void deselect();

        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();

        bool isSelectable() const;

    private:
        bool mIsSelectable = false;
        bool mIsSelected = false;
        bool mIsActive = false;
    };

}