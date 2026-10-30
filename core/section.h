#pragma once

#include "../rendering/rendering.h"

// The base Section class.
class Section {
public:

    Vector2 pos;
    Vector2 size;

    virtual void update() {};
    virtual void draw(Vector2 pos_offset) {};
};
