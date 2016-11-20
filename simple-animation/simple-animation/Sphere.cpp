#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(int radius)
    : mRadius(radius)
{}

int Sphere::radius() const
{
    return mRadius;
}

bool Sphere::operator == (const Sphere & sphere) const
{
    return mRadius == sphere.mRadius;
}

bool Sphere::operator != (const Sphere & sphere) const
{
    return !(*this == sphere);
}