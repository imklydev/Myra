#pragma once

#include "../section.h"

class projectFolderOverview : public Section
{
public:
    Vector2 pos = {0, 0};
    Vector2 size = {170, 5000};
    float textSize = 16;

    void draw(Vector2 pos_offset) override
    {
        SetDrawColor(TABS_SECTION_TAB_COLOR);
        DrawRect(pos + pos_offset, size);

        SetDrawColor(TEXT_COLOR);
        DrawText("Nothing to\nsee here yet...", 16, {15, 50});
    }
};

inline projectFolderOverview ProjectFolderOverview;
