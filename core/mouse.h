#pragma once

#include <SDL3/SDL.h>
#include "../rendering/misc.h"

#include <string>

struct MouseState
{
    bool leftClicked = false;
    bool rightClicked = false;
    Vector2 position;
    float scroll = 0;
};

extern MouseState Mouse;

void BeginMouseFrame();

void HandleMouse(SDL_Event *event);

bool isMouseHoveringOver(Vector2 pos, Vector2 size);