#pragma once

#include "Math.h"
#include "Sphere.h"

class Tetrahedron
{
public:
    Tetrahedron(const Sphere & shere);
    Tetrahedron(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec3 const& p4);

    glm::vec3 p1() const;
    glm::vec3 p2() const;
    glm::vec3 p3() const;
    glm::vec3 p4() const;

    std::vector<glm::vec3> getVertices() const;

private:
    glm::vec3 mP1;
    glm::vec3 mP2;
    glm::vec3 mP3;
    glm::vec3 mP4;
};
