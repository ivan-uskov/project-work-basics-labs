#pragma once

#include "ITransformation.h"

#include "Math.h"

class Colorer : public ITransformation
{
public:
    Colorer(std::vector<glm::u8vec4> const& colors);

    virtual void update(sf::Time msec) override;
    virtual void accept(Math::Vertex & vertex) const override;

private:
    std::vector<glm::u8vec4> m_colors;
};

