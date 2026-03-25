#include <Renderer.h>
#include "Renderer.h"
#include "Config.h"

Renderer::Renderer() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
{
}

void Renderer::Setup()
{
    // Próbujemy zainicjować ekran. Adres 0x3C to standardowy adres domowy tego modelu OLED. Instrukcja if sprawdza, czy ekran odpowiedział na nasze wezwanie.
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("BLAD: Nie znaleziono ekranu OLED!");
        for (;;)
            ; // Jeśli nie ma ekranu, wpadamy w nieskończoną pętlę i zawieszamy program.
    }
    display.clearDisplay(); // Jeśli wszystko się udało, czyścimy "bufor" ekranu (pamięć przygotowującą klatkę do wyświetlenia).
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    // display.display(); // Na koniec wysyłamy przygotowany bufor z pamięci mikrokontrolera na fizyczny ekran.
}

void Renderer::Display()
{
    display.display();
}

void Renderer::ClearDisplay()
{
    display.clearDisplay();
}
