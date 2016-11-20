#include "stdafx.h"
#include "Colorer.h"

#include <random>

size_t shootYourselfInTheFoot(size_t mod) {
    static int pos = 0;

    if (mod == 0)
    {
        pos = 0;
        return 0;
    }

    return pos++ % mod;
}


Colorer::Colorer(std::vector<glm::u8vec4> const& colors)
    : m_colors(colors)
{
}

void Colorer::update(sf::Time msec)
{
    shootYourselfInTheFoot(0);
}

void Colorer::accept(Math::Vertex & vertex) const
{
    if (m_colors.empty())
    {
        vertex.color = { 0, 0, 0, 255 };
        return;
    }

    auto color = m_colors[shootYourselfInTheFoot(m_colors.size())];
    vertex.color = { color.r, color.g, color.b, color.a };

}
