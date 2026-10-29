#pragma once

#include "../rendering/rendering.h"

// The base Screen class.
class Screen {
public:
    virtual void update() {};
    virtual void draw() {};
};
