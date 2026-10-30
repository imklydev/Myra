#include "mouse.h"

#include <iostream>

MouseState Mouse;

void BeginMouseFrame()
{
    Mouse.leftClicked = false;
    Mouse.rightClicked = false;
    Mouse.scroll = 0;
}

void HandleMouse(SDL_Event *event)
{
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            Mouse.leftClicked = true;
        }
        else if (event->button.button == SDL_BUTTON_RIGHT)
        {
            Mouse.rightClicked = true;
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_WHEEL)
    {
        Mouse.scroll = event->wheel.y;
    }
}