#include "../rendering/rendering.h"
#include "../core/mouse.h"

class Button
{

public:
    Vector2 pos;
    Vector2 size;
    void (*onClick)(void);

    void update()
    {
        if (Mouse.leftClicked)
        {
            if (Mouse.position.x >= pos.x && Mouse.position.x <= pos.x + size.x) // on the right position on the x-axis
            {
                if (Mouse.position.y >= pos.y && Mouse.position.y <= pos.y + size.y) // on the right position on the y-axis
                {
                    onClick();
                }
            }
        }
    }

    void draw()
    {
        update();
        DrawRect(pos, size);
    };
};
