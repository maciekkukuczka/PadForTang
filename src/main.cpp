#include <Arduino.h>
#include <Wire.h> // Biblioteka do obsługi naszej "rury z danymi" - I2C
#include "Config.h"
#include "Renderer.h"
#include "Text.h"
#if USE_XBOX_PAD
#include "Pad.h"
Pad pad;
#endif
Renderer renderer;
Text text(renderer);

void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("SETUP");
    text.ShowText("SETUP");
    Wire.begin(20, 21);
    delay(500);
    renderer.Setup();
    delay(500);
    text.ShowText("SETUP Ukonczony!");
    renderer.Display();
    delay(1000);

#if USE_XBOX_PAD
    pad.StartControllerRadar();
    Serial.println("BT Właczony...");
    text.ShowText("BT Właczony...");
    delay(1000);

    Serial.println("START");
    text.ShowText("START");
    delay(1000);

#endif
}

void loop()
{

#if USE_XBOX_PAD
    pad.ControllerUpdate();
    /*     if (pad.Connected() && pad.padWasConnected == false)
        {
            Serial.println("Pad XBOX połączony!");
            text.ShowText("Pad XBOX połączony!");
            pad.padWasConnected = true;
            delay(500);
        }
        else if (pad.Connected() == false && pad.padWasConnected == true)
        {
            Serial.println("Pad XBOX rozłączony!");
            text.ShowText("Pad XBOX rozłączony!");
            pad.padWasConnected = false;
            delay(500);

        } */
    if (pad.Connected())
    {
        Serial.println("Pad XBOX polaczony!");
        text.ShowText("Pad XBOX polaczony!");

        if (pad.UpPressed())
        {
            text.ShowText("Wcisnieto: GORA", 0, 16, false);
        }
        if (pad.DownPressed())
        {
            text.ShowText("Wcisnieto: DOŁ", 0, 16, false);
        }
        if (pad.LeftPressed())
        {
            text.ShowText("Wcisnieto: LEWO", 0, 16, false);
        }
        if (pad.RightPressed())
        {
            text.ShowText("Wcisnieto: PRAWO", 0, 16, false);
        }
        if (pad.StartPressed())
        {
            text.ShowText("Wcisnieto: START", 0, 16, false);
        }
        if (pad.SelectPressed())
        {
            text.ShowText("Wcisnieto: SELECT", 0, 16, false);
        }
    }
    else
    {
        Serial.println("Pad XBOX rozlaczony!");
        text.ShowText("Pad XBOX rozlaczony!");
    }
#endif

    renderer.Display();
    delay(10);
}