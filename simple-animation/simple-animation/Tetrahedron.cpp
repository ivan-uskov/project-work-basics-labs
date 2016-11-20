#include "stdafx.h"
#include "tetrahedron.h"

using namespace glm;

Tetrahedron::Tetrahedron(const Sphere & shere)
{
    const auto angleBetweenRadius = float(M_PI * 2 / 3);
    auto rotationY = glm::rotate(angleBetweenRadius, Math::Y_AXIS);
    auto rotationX = glm::rotate(angleBetweenRadius, Math::X_AXIS);

    mP1 = Math::Y_AXIS * float(shere.radius());
    mP2 = rotationX * vec4(mP1, 0.f);
    mP3 = rotationY * vec4(mP2, 0.f);
    mP4 = rotationY * vec4(mP3, 0.f);
}

Tetrahedron::Tetrahedron(vec3 const& p1, vec3 const& p2, vec3 const& p3, vec3 const& p4)
    : mP1(p1)
    , mP2(p2)
    , mP3(p3)
    , mP4(p4)
{
}

vec3 Tetrahedron::p1() const
{
    return mP1;
}

vec3 Tetrahedron::p2() const
{
    return mP2;
}

vec3 Tetrahedron::p3() const
{
    return mP3;
}

vec3 Tetrahedron::p4() const
{
    return mP4;
}

std::vector<vec3> Tetrahedron::getVertices() const
{
    return
    {
        mP1,
        mP2,
        mP3,
        mP4
    };
}
