#include "mouse.h"

#include <iostream>

MouseState Mouse;

void BeginMouseFrame()
{
    Mouse.leftClicked = false;
    Mouse.rightClicked = false;
    Mouse.scroll = 0;

    SDL_GetMouseState(&Mouse.position.x, &Mouse.position.y);
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

bool isMouseHoveringOver(Vector2 pos, Vector2 size)
{
    if (Mouse.position.x >= pos.x && Mouse.position.x <= pos.x + size.x) // on the right position on the x-axis
    {
        if (Mouse.position.y >= pos.y && Mouse.position.y <= pos.y + size.y) // on the right position on the y-axis
        {
            return true;
        }
    }

    return false;
}