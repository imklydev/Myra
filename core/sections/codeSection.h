#pragma once

#include "../section.h"

class codeSection : public Section
{
private:
    std::vector<TextSpan> GetTextColoringSpans()
    {
        std::vector<TextSpan> textSpans;
        for (size_t i = 0; i < text.size(); i++)
        {
            char c = text[i];

            // if a character is a number, make it green (just a test to see if text coloring works)
            if (c >= '0' && c <= '9')
            {
                TextSpan span;
                size_t start = i;

                while (c >= '0' && c <= '9')
                {
                    i++;
                    c = text[i];
                }

                span.start = start;
                span.end = i;
                span.color = LIGHT_GREEN;
                textSpans.push_back(span);
            }
        }

        return textSpans;
    }

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
        DrawText(text, textSize, pos + pos_offset, size, GetTextColoringSpans());
    }
};

inline codeSection CodeSection;
