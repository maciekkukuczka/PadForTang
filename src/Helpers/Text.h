#pragma once
#include <WString.h>
#include "Renderer.h"

class Text
{
private:
    Renderer& renderer;
    public:
    Text(Renderer&);

    void ShowText(String text, int x = 0, int y = 0, bool clearScreen = true);
};
