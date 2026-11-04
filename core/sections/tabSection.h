#pragma once

#include <vector>
#include "../section.h"

struct textSnapshot
{
    std::string text;
    int CursorPos = -1; // the character the cursor is selecting (-1 = begining, before any char)
};

struct codeTab
{
    std::vector<textSnapshot> textSnapshots; // for ctrl z
    int undoHistoryIndex = 0;
    std::string text;
    std::string openedFile = "";
    int CursorPos = -1; // the character the cursor is selecting (-1 = begining, before any char)
    int SelectionStartPos = -1;
    int selectionLength;
    bool selectionMode = false;
    Vector2 offset = {0, 0};

    // this was pretty unnecesary but i needed it for one if statement lol
    bool operator==(const codeTab &other) const
    {
        return undoHistoryIndex == other.undoHistoryIndex &&
               text == other.text &&
               openedFile == other.openedFile &&
               CursorPos == other.CursorPos &&
               SelectionStartPos == other.SelectionStartPos &&
               selectionLength == other.selectionLength &&
               selectionMode == other.selectionMode &&
               offset.x == other.offset.x &&
               offset.y == other.offset.y;
    }
};

class tabSection : public Section
{

public:
    Vector2 pos = {0, 0};
    Vector2 size = {(float)ScreenWidth, 30};
    int textSize = 16;
    float tabSize = 120;
    float tabSpacing = 5;
    std::vector<codeTab> *codeTabs;

    void draw(Vector2 pos_offset, int selectedTab = 0)
    {
        SetDrawColor(TABS_SECTION_BG_COLOR);
        DrawRect(pos, size); // Background (tabs are drawn on top of this)

        for (int i = 0; i < codeTabs->size(); i++)
        {
            if (i == selectedTab)
            {
                SetDrawColor(TABS_SECTION_SELECTED_TAB_COLOR);
            }
            else
            {
                SetDrawColor(TABS_SECTION_TAB_COLOR);
            }

            DrawRect(pos + Vector2{tabSize * i + tabSpacing * i, 0}, {tabSize, size.y});

            if ((*codeTabs)[i].openedFile.empty())
            {
                SetDrawColor(TEXT_COLOR);
                DrawText("Untitled", textSize, pos + Vector2{tabSize * i + tabSpacing * i + 5.0f, 0.0f}, {tabSize, size.y});
            }
            else
            {
                SetDrawColor(TEXT_COLOR);
                std::string path = (*codeTabs)[i].openedFile;
                std::string title = path.substr(path.find_last_of("/\\") + 1);
                DrawText(title, textSize, pos + Vector2{tabSize * i + tabSpacing * i + 0.0f, 0.0f}, {tabSize, size.y});
            }
        }
    }
};

tabSection TabSecton;
