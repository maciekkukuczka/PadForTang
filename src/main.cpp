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
    Wire.begin(20,21);
    delay(500);
    renderer.Setup();
    delay(500);
    text.ShowText("SETUP Complete!");
    renderer.Display();
    delay(1000);
    
    
    #if USE_XBOX_PAD
    pad.StartControllerRadar();
    text.ShowText("BT Włączony...");

    delay(1000);

    #endif
}

void loop()
{
    Serial.println("START");
    text.ShowText("START");
    delay(1000);

#if USE_XBOX_PAD
    pad.ControllerUpdate();
    if (pad.Connected() && pad.padWasConnected == false)
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

    }
#endif

    renderer.Display();
    delay(100);
}