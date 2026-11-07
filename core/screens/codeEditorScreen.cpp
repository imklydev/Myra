#include "codeEditorScreen.h"

void ToggleShowProjectFolderOverview()
{
    CodeEditorScreen.shouldShowProjectFolderOverview = !CodeEditorScreen.shouldShowProjectFolderOverview;
}
std::string GetOpenedFolder()
{
    return CodeEditorScreen.openedFolder;
}

std::string GetOpenedFile()
{
    return CodeEditorScreen.codeTabs[CodeEditorScreen.currentTab].openedFile;
}

void OpenTab(char *file)
{
    CodeEditorScreen.OpenTab(file);
}

void SetCurrentTab(int tab)
{
    CodeEditorScreen.SetCurrentTab(tab);
}