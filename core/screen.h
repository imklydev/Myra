#pragma once

#include "../rendering/rendering.h"

// The base Screen class.
class Screen
{
public:
    virtual void update() {};
    virtual void draw() {};
};

inline Screen *currentScreen;
inline Screen *nextScreen;

extern void SetCurrentScreen(Screen *targetScreen);

// Don't use this in draw() or update() loops from screen classes. Use SetCurrentScreen() instead.
extern void ActuallySetCurrentScreen();