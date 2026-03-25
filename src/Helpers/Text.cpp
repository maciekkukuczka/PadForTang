#include "Text.h"


Text::Text(Renderer& r):renderer(r){}

void Text::ShowText(String text, int x, int y, bool clearDisplay)
{
    if (clearDisplay)
    {
        renderer.display.clearDisplay();
    }
    
    renderer.display.setCursor(x,y);
    renderer.display.println(text);
    // renderer.display.display();


}


