#pragma once

#include <SDL3/SDL.h>
#include <string>

inline std::string FOLDER_ICON_PATH = std::string(SDL_GetBasePath()) + "assets/icons/folderIcon.png";
inline std::string COLLAPSE_FOLDER_ICON = std::string(SDL_GetBasePath()) + "assets/icons/collapseFolderIcon.png";
inline std::string DROPDOWN_FOLDER_ICON = std::string(SDL_GetBasePath()) + "assets/icons/dropdownFolderIcon.png";