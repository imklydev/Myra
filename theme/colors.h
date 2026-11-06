#pragma once

struct Color
{
    int r, g, b, a;
};

inline Color BG_COLOR = {5, 5, 15, 255};
inline Color SELECTION_BG_COLOR = {255, 255, 255, 255};
inline Color TEXT_COLOR = {225, 225, 245, 255};
inline Color LINE_NUMBERS_COLOR = {12, 9, 25, 255};
inline Color TABS_SECTION_BG_COLOR = {12, 9, 25, 255};
inline Color TABS_SECTION_TAB_COLOR = {15, 15, 25, 255};
inline Color TABS_SECTION_SELECTED_TAB_COLOR = {35, 35, 55, 255};
inline Color TABS_SECTION_HOVERED_TAB_COLOR = {28, 28, 45, 255};

inline SDL_Color LIGHT_GREEN = {85, 150, 85, 255};