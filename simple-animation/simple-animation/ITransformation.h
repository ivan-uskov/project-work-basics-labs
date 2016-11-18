#pragma once

#include <SFML/System/Time.hpp>
#include "Math.h"

class ITransformation
{
public:
    virtual void update(sf::Time msec) = 0;
    virtual void accept(Math::Vertex & vertex) const = 0;
};