#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../theme/theme.h"
#include "misc.h"

// Color
void SetDrawColor(Color color);

// Shapes
void DrawRect(Vector2 pos, Vector2 size, bool filled = true);

// Images
void DrawImage(Vector2 pos, Vector2 size, const char *path);

void FreeImage(const char *path);

void FreeAllImages();

// Text
extern std::unordered_map<std::string, TTF_Font *> cachedFonts;

struct TextSpan
{
    size_t start; // byte offset into the full text
    size_t end;   // exclusive
    SDL_Color color;
};

extern TTF_Font *GetFont(int textSize);

void DrawText(const std::string &text, int textSize, Vector2 pos, Vector2 size = {-1, -1}, const std::vector<TextSpan> &spans = {});

void FreeText(const std::string &text, int textSize, const std::vector<TextSpan> &spans = {});

void FreeAllText();

void FreeAllFonts();
