#pragma once

#include <SDL3/SDL.h>

#include <string>

struct MouseState
{
    bool leftClicked = false;
    bool rightClicked = false;
    float scroll = 0;
};

extern MouseState Mouse;

void BeginMouseFrame();

void HandleMouse(SDL_Event *event);