#pragma once

#include "../section.h"

class codeSection : public Section
{

public:
    Vector2 pos = {60, 0};
    Vector2 size = {4000, 40000};
    float textSize = 16;
    std::string text = "";

    void draw(Vector2 pos_offset) override
    {
        SetDrawColor(BG_COLOR);
        DrawRect(pos + pos_offset, size);

        SetDrawColor(TEXT_COLOR);
        DrawText(text, textSize, pos + pos_offset, size);
    }
};

codeSection CodeSection;
