#pragma once

#include <algorithm>
#include <cmath>

struct Vector2
{
    float x, y;

    bool operator==(const Vector2 &other) const
    {
        return x == other.x && y == other.y;
    }

    Vector2 operator+(const Vector2 &other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return {x - other.x, y - other.y};
    }

    Vector2 &operator+=(const Vector2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2 &operator-=(const Vector2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

struct Vector2i
{
    int x, y;

    bool operator==(const Vector2i &other) const
    {
        return x == other.x && y == other.y;
    }

    Vector2i operator+(const Vector2i &other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector2i operator-(const Vector2i &other) const
    {
        return {x - other.x, y - other.y};
    }

    Vector2i &operator+=(const Vector2i &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2i &operator-=(const Vector2i &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};