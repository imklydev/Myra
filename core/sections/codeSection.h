#pragma once

#include "../section.h"

class codeSection : public Section
{
public:
    Vector2 pos = {60, 0};
    Vector2 size = {740, 800};
    float textSize = 16;
    std::string text = "";

    void draw() override
    {
        SetDrawColor(BG_COLOR);
        DrawRect(pos, size);

        SetDrawColor(TEXT_COLOR);
        DrawText(text, textSize, pos, size);
    }
};

codeSection CodeSection;
