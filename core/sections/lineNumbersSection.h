#pragma once

#include "../section.h"

class lineNumbersSection : public Section
{
public:
    Vector2 pos = {0, 0};
    Vector2 size = {50, 40000};
    float textSize = 16;
    int lineCount = 1;
    std::string lineNumbers;

    void setLineCount(int count)
    {
        if (count == lineCount && !lineNumbers.empty())
        {
            return;
        }

        lineCount = count;
        lineNumbers.clear();

        for (int i = 1; i <= lineCount; ++i)
        {
            lineNumbers += std::to_string(i);

            if (i != lineCount)
            {
                lineNumbers += '\n';
            }
        }
    }

    void draw(Vector2 pos_offset) override
    {
        if (lineNumbers.empty())
        {
            setLineCount(lineCount);
        }

        SetDrawColor(LINE_NUMBERS_COLOR);
        DrawRect(pos + pos_offset, size);

        SetDrawColor(TEXT_COLOR);
        DrawText(lineNumbers, textSize, pos + pos_offset + Vector2{5, 0}, size);
    }
};

lineNumbersSection LineNumbersSection;
