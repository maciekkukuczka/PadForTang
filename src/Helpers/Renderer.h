#pragma once
#include <Wire.h>             // Biblioteka do obsługi naszej "rury z danymi" - I2C
#include <Adafruit_GFX.h>     // Główna biblioteka graficzna (do rysowania tekstów, kółek)
#include <Adafruit_SSD1306.h> // Konkretny sterownik dla Twojego ekranu OLED

class Renderer
{
    public:
    Adafruit_SSD1306 display;
    Renderer();
    void Setup();
    void Display();
    void ClearDisplay();


    private:
};

