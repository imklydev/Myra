#pragma once

#include <algorithm>

struct Vector2
{
    float x, y;

    bool operator==(const Vector2 &other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Vector2i
{
    int x, y;

    bool operator==(const Vector2i &other) const
    {
        return x == other.x && y == other.y;
    }
};