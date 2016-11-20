#pragma once

class Sphere
{
public:
    Sphere(int radius);

    int radius() const;

    bool operator == (const Sphere & sphere) const;
    bool operator != (const Sphere & sphere) const;

private:
    int mRadius;
};

