#include "screen.h"


void ActuallySetCurrentScreen()
{
    currentScreen = nextScreen;
}


void SetCurrentScreen(Screen *targetScreen)
{
    nextScreen = targetScreen;
}