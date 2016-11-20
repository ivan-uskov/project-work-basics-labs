#include "stdafx.h"
#include "Cube.h"

Cube::Cube(int length)
    : mLength(length)
{}

int Cube::length() const
{
    return mLength;
}
