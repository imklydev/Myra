#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../theme/theme.h"
#include "misc.h"

// Color
void SetDrawColor(Color color);

// Shapes
void DrawRect(Vector2 pos, Vector2 size, bool filled = true);

// Text
extern std::unordered_map<std::string, TTF_Font *> cachedFonts;

void DrawText(const std::string &text, int textSize, Vector2 pos, Vector2 size = {-1, -1});

void FreeText(const std::string &text, int textSize);

void FreeAllText();

void FreeAllFonts();
