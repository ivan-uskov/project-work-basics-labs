#include "stdafx.h"
#include "Component.h"

namespace GUI
{
    Component::Component(bool isSelectable)
        : mIsSelectable(isSelectable)
    {}

    bool Component::isSelectable() const
    {
        return mIsSelectable;
    }

    bool Component::isSelected() const
    {
        return mIsSelected;
    }

    void Component::select()
    {
        mIsSelected = true;
    }

    void Component::deselect()
    {
        mIsSelected = false;
    }

    bool Component::isActive() const
    {
        return mIsActive;
    }

    void Component::activate()
    {
        mIsActive = true;
    }

    void Component::deactivate()
    {
        mIsActive = false;
    }
}