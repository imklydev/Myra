#include "rendering.h"


// Color
void SetDrawColor(Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// Shapes
void DrawRect(Vector2 pos, Vector2 size, bool filled)
{
    SDL_FRect rect = {pos.x, pos.y, size.x, size.y};

    if (filled)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderRect(renderer, &rect);
    }
}

// Images

namespace
{
    struct CachedImage
    {
        SDL_Texture *texture = nullptr;
        float width = 0;
        float height = 0;
    };

    std::unordered_map<std::string, CachedImage> cachedImages;

    const CachedImage *GetImage(const char *path)
    {
        auto it = cachedImages.find(path);
        if (it != cachedImages.end())
        {
            return it->second.texture ? &it->second : nullptr;
        }

        CachedImage image;
        image.texture = IMG_LoadTexture(renderer, path);
        SDL_SetTextureScaleMode(image.texture, SDL_SCALEMODE_NEAREST);
        // i'm ashamed of this line of code lmao. i needed this for ONE texture and was too lazy to implement a better option

        if (image.texture)
        {
            SDL_GetTextureSize(image.texture, &image.width, &image.height);
        }

        // Cache failures too (texture == nullptr) so a missing file
        // doesn't hit the disk every frame.
        auto inserted = cachedImages.emplace(path, image);
        return image.texture ? &inserted.first->second : nullptr;
    }
}

void DrawImage(Vector2 pos, Vector2 size, const char *path)
{
    if (!path)
    {
        return;
    }

    const CachedImage *image = GetImage(path);
    if (!image)
    {
        return;
    }

    // Negative size means "use the image's native size"
    float w = size.x >= 0 ? size.x : image->width;
    float h = size.y >= 0 ? size.y : image->height;

    SDL_FRect rect = {pos.x, pos.y, w, h};
    SDL_RenderTexture(renderer, image->texture, nullptr, &rect);
}

void FreeImage(const char *path)
{
    auto it = cachedImages.find(path);
    if (it == cachedImages.end())
    {
        return;
    }

    if (it->second.texture)
    {
        SDL_DestroyTexture(it->second.texture);
    }

    cachedImages.erase(it);
}

void FreeAllImages()
{
    for (auto &entry : cachedImages)
    {
        if (entry.second.texture)
        {
            SDL_DestroyTexture(entry.second.texture);
        }
    }

    cachedImages.clear();
}

// Text

std::unordered_map<std::string, TTF_Font *> cachedFonts;

TTF_Font *GetFont(int textSize)
{
    std::string fontKey = FONT + "_" + std::to_string(textSize);

    auto it = cachedFonts.find(fontKey);
    if (it != cachedFonts.end())
    {
        return it->second;
    }

    TTF_Font *font = TTF_OpenFont(FONT.c_str(), textSize);
    if (!font)
    {
        return nullptr;
    }

    cachedFonts[fontKey] = font;
    return font;
}

namespace
{
    struct LineKey
    {
        std::string line;
        int textSize;
        Uint8 r, g, b, a;

        bool operator==(const LineKey &other) const
        {
            return textSize == other.textSize && r == other.r && g == other.g &&
                   b == other.b && a == other.a && line == other.line;
        }
    };

    struct LineKeyHash
    {
        size_t operator()(const LineKey &key) const noexcept
        {
            size_t hash = std::hash<std::string>{}(key.line);

            auto combine = [&hash](size_t value)
            {
                hash ^= value + 0x9e3779b97f4a7c15ULL + (hash << 6) + (hash >> 2);
            };

            combine(std::hash<int>{}(key.textSize));
            combine(key.r);
            combine(key.g);
            combine(key.b);
            combine(key.a);

            return hash;
        }
    };

    struct CachedLine
    {
        SDL_Texture *texture = nullptr;
        float width = 0;
        float height = 0;
        size_t bytes = 0;
        uint64_t lastUsed = 0;
    };

    constexpr size_t MAX_CACHED_LINE_BYTES = 32 * 1024 * 1024;

    std::unordered_map<LineKey, CachedLine, LineKeyHash> cachedLines;
    size_t cachedLineBytes = 0;
    uint64_t textUseClock = 0;

    void EvictLines()
    {
        while (cachedLineBytes > MAX_CACHED_LINE_BYTES && cachedLines.size() > 1)
        {
            auto victim = cachedLines.begin();

            for (auto it = cachedLines.begin(); it != cachedLines.end(); ++it)
            {
                if (it->second.lastUsed < victim->second.lastUsed)
                {
                    victim = it;
                }
            }

            cachedLineBytes -= victim->second.bytes;
            SDL_DestroyTexture(victim->second.texture);
            cachedLines.erase(victim);
        }
    }

    SDL_Texture *GetLineTexture(
        TTF_Font *font, const LineKey &key, float &width, float &height)
    {
        auto it = cachedLines.find(key);
        if (it != cachedLines.end())
        {
            it->second.lastUsed = ++textUseClock;
            width = it->second.width;
            height = it->second.height;
            return it->second.texture;
        }

        SDL_Color color = {key.r, key.g, key.b, key.a};
        SDL_Surface *surface =
            TTF_RenderText_Blended(font, key.line.c_str(), key.line.size(), color);

        if (!surface)
        {
            return nullptr;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (!texture)
        {
            return nullptr;
        }

        float textureWidth = 0;
        float textureHeight = 0;
        SDL_GetTextureSize(texture, &textureWidth, &textureHeight);

        CachedLine cachedLine;
        cachedLine.texture = texture;
        cachedLine.width = textureWidth;
        cachedLine.height = textureHeight;
        cachedLine.bytes =
            static_cast<size_t>(textureWidth) * static_cast<size_t>(textureHeight) * 4;
        cachedLine.lastUsed = ++textUseClock;

        cachedLineBytes += cachedLine.bytes;
        cachedLines.emplace(key, cachedLine);

        width = textureWidth;
        height = textureHeight;
        return texture;
    }

    // Splits one line into (begin, end, color) segments, using spans for colored
    // parts and `def` for the gaps. `spanIdx` is a cursor that persists between
    // lines, so total work is O(lines + spans) rather than O(lines * spans).
    // Both DrawText and FreeText use this, so they always produce identical
    // cache keys.
    template <typename Emit>
    void ForEachSegment(
        const std::string &line, size_t lineOffset,
        const std::vector<TextSpan> &spans, size_t &spanIdx,
        SDL_Color def, Emit &&emit)
    {
        const size_t lineEnd = lineOffset + line.size();

        // Skip spans that ended before this line (they can never matter again)
        while (spanIdx < spans.size() && spans[spanIdx].end <= lineOffset)
        {
            ++spanIdx;
        }

        size_t cursor = 0;

        for (size_t i = spanIdx; i < spans.size() && spans[i].start < lineEnd; ++i)
        {
            size_t s = std::max(spans[i].start, lineOffset) - lineOffset;
            size_t e = std::min(spans[i].end, lineEnd) - lineOffset;

            if (s < cursor)
            {
                s = cursor; // tolerate accidental overlap
            }
            if (s >= e)
            {
                continue;
            }

            if (cursor < s)
            {
                emit(cursor, s, def);
            }
            emit(s, e, spans[i].color);
            cursor = e;
        }

        if (cursor < line.size())
        {
            emit(cursor, line.size(), def);
        }
    }
}

void DrawText(const std::string &text, int textSize, Vector2 pos, Vector2 size,
              const std::vector<TextSpan> &spans)
{
    SDL_Color color;
    SDL_GetRenderDrawColor(
        renderer, &color.r, &color.g, &color.b, &color.a);

    TTF_Font *font = GetFont(textSize);
    if (!font)
    {
        return;
    }

    int lineSkip = TTF_GetFontLineSkip(font);

    bool clip = size.x >= 0 && size.y >= 0;
    bool hadClip = clip && SDL_RenderClipEnabled(renderer);
    SDL_Rect previousClip = {0, 0, 0, 0};

    if (clip)
    {
        if (hadClip)
        {
            SDL_GetRenderClipRect(renderer, &previousClip);
        }

        SDL_Rect clipRect = {
            static_cast<int>(pos.x),
            static_cast<int>(pos.y),
            static_cast<int>(size.x),
            static_cast<int>(size.y)};

        SDL_SetRenderClipRect(renderer, &clipRect);
    }

    float y = pos.y;
    size_t start = 0;
    size_t spanIdx = 0;

    while (start <= text.size())
    {
        size_t end = text.find('\n', start);
        if (end == std::string::npos)
        {
            end = text.size();
        }

        std::string line = text.substr(start, end - start);
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            float x = pos.x;

            ForEachSegment(
                line, start, spans, spanIdx, color,
                [&](size_t a, size_t b, SDL_Color c)
                {
                    LineKey key = {
                        line.substr(a, b - a), textSize, c.r, c.g, c.b, c.a};

                    float width = 0;
                    float height = 0;
                    SDL_Texture *texture = GetLineTexture(font, key, width, height);

                    if (texture)
                    {
                        SDL_FRect rect = {x, y, width, height};
                        SDL_RenderTexture(renderer, texture, nullptr, &rect);
                        x += width;
                    }
                });
        }

        if (end == text.size())
        {
            break;
        }

        start = end + 1;
        y += lineSkip;

        if (clip && y >= pos.y + size.y)
        {
            break;
        }
    }

    if (clip)
    {
        if (hadClip)
        {
            SDL_SetRenderClipRect(renderer, &previousClip);
        }
        else
        {
            SDL_SetRenderClipRect(renderer, nullptr);
        }
    }

    EvictLines();
}

void FreeText(const std::string &text, int textSize,
              const std::vector<TextSpan> &spans)
{
    // Regenerate the same segments DrawText would have produced and free
    // matching cache entries (colour is ignored, as before).
    std::unordered_set<std::string> segments;
    SDL_Color unused = {0, 0, 0, 0};
    size_t spanIdx = 0;
    size_t start = 0;

    while (start <= text.size())
    {
        size_t end = text.find('\n', start);
        if (end == std::string::npos)
        {
            end = text.size();
        }

        std::string line = text.substr(start, end - start);
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            ForEachSegment(
                line, start, spans, spanIdx, unused,
                [&](size_t a, size_t b, SDL_Color)
                {
                    segments.insert(line.substr(a, b - a));
                });
        }

        if (end == text.size())
        {
            break;
        }

        start = end + 1;
    }

    for (auto it = cachedLines.begin(); it != cachedLines.end();)
    {
        const LineKey &key = it->first;

        if (key.textSize == textSize && segments.find(key.line) != segments.end())
        {
            cachedLineBytes -= it->second.bytes;
            SDL_DestroyTexture(it->second.texture);
            it = cachedLines.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void FreeAllText()
{
    for (auto &entry : cachedLines)
    {
        SDL_DestroyTexture(entry.second.texture);
    }

    cachedLines.clear();
    cachedLineBytes = 0;
}

void FreeAllFonts()
{
    for (auto &entry : cachedFonts)
    {
        TTF_CloseFont(entry.second);
    }

    cachedFonts.clear();
}