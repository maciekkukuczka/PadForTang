#include <Arduino.h>
#include <Wire.h> // Biblioteka do obsługi naszej "rury z danymi" - I2C
#include "Config.h"
#include "Renderer.h"
#include "Text.h"
#include "VirtualNesPad.h"
#if USE_XBOX_PAD
#include "Pad.h"
Pad pad;
VirtualNesPad nesPadPlayer1(pad, 3,4,5);
// VirtualNesPad nesPadPlayer2(pad,14,15,18);
#endif
Renderer renderer;
Text text(renderer);

// --- FUNKCJE POŚREDNICZĄCE DLA PRZERWAŃ ---
// To one są podpinane pod piny, a same tylko "przekazują piłkę" do klasy.

void IRAM_ATTR latchWrapper1(){
    nesPadPlayer1.OnLatchRising();
}
void IRAM_ATTR clockWrapper1(){
    nesPadPlayer1.OnClockRising();
}
/* void IRAM_ATTR latchWrapper2(){
    nesPadPlayer2.OnLatchRising();
}
void IRAM_ATTR clockWrapper2(){
    nesPadPlayer2.OnClockRising();
} */


void setup()
{
    Serial.begin(115200);
    Wire.begin(20, 21);

    // Konfiguracja kierunków (kto słucha, a kto nadaje)
    pinMode(nesPadPlayer1.PIN_CLOCK, INPUT); // ESP słucha zegara
    pinMode(nesPadPlayer1.PIN_LATCH, INPUT); // ESP słucha zatrzasku
    pinMode(nesPadPlayer1.PIN_DATA, OUTPUT); // ESP nadaje dane
/*     pinMode(nesPadPlayer2.PIN_CLOCK, INPUT); // ESP słucha zegara
    pinMode(nesPadPlayer2.PIN_LATCH, INPUT); // ESP słucha zatrzasku
    pinMode(nesPadPlayer2.PIN_DATA, OUTPUT); */

    // Na start ustawiamy na linii Danych stan wysoki (puszczony przycisk)
    digitalWrite(nesPadPlayer1.PIN_DATA, HIGH);
    // digitalWrite(nesPadPlayer2.PIN_DATA, HIGH);

     // Podpinamy naszą funkcję onLatchRising pod żółty kabel.
        // RISING oznacza, że funkcja odpali się dokładnie w tym ułamku mikrosekundy,
        // gdy napięcie na kablu skoczy z 0V na 3.3V. 
    attachInterrupt(digitalPinToInterrupt(nesPadPlayer1.PIN_LATCH), latchWrapper1, RISING);
    // Podpinamy funkcję zegara pod pomarańczowy kabel
    attachInterrupt(digitalPinToInterrupt(nesPadPlayer1.PIN_CLOCK), clockWrapper1, FALLING);

    // attachInterrupt(digitalPinToInterrupt(nesPadPlayer2.PIN_LATCH), latchWrapper2, RISING);
    // attachInterrupt(digitalPinToInterrupt(nesPadPlayer2.PIN_CLOCK), clockWrapper2, FALLING);

    renderer.Setup();

    Serial.println("Konfiguracja...");
    text.ShowText("Konfiguracja...");
    renderer.Display();
    delay(800);

#if USE_XBOX_PAD
    Serial.println("Inicjalizacja BT...");
    text.ShowText("Inicjalizacja BT...", 0, 10, false);
    renderer.Display();

    pad.StartControllerRadar();

    Serial.println("[ OK ]");
    text.ShowText("[ OK ]", 100, 10, false);
    renderer.Display();
    delay(800);

#endif

    Serial.println("System GOTOWY!");
    text.ShowText("System GOTOWY!", 0, 20, false);
    renderer.Display();
    delay(1000);
}
 


void loop()
{

#if USE_XBOX_PAD
    pad.ControllerUpdate();
    nesPadPlayer1.UpdateState();
    // nesPadPlayer2.UpdateState();

    if (pad.Connected())
    {
        Serial.println("Pad POLACZONY!");
        text.ShowText("Pad POLACZONY!");

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
        if (pad.APressed())
        {
            text.ShowText("Wcisnieto: A", 0, 16, false);
        }
        if (pad.BPressed())
        {
            text.ShowText("Wcisnieto: B", 0, 16, false);
        }
        if (pad.XPressed())
        {
            text.ShowText("Wcisnieto: X", 0, 16, false);
        }
        if (pad.YPressed())
        {
            text.ShowText("Wcisnieto: Y", 0, 16, false);
        }
        if (pad.StartPressed())
        {
            text.ShowText("Wcisnieto: START", 0, 16, false);
        }
        if (pad.SelectPressed())
        {
            text.ShowText("Wcisnieto: SELECT", 0, 16, false);
        }
        if (pad.SharePressed())
        {
            text.ShowText("Wcisnieto: SHARE", 0, 16, false);
        }
        if (pad.XBoxPressed())
        {
            text.ShowText("Wcisnieto: XBOX", 0, 16, false);
        }
    }
    else
    {
        Serial.println("Pad ROZLACZONY!");
        text.ShowText("Pad ROZLACZONY!");
    }
#endif

    renderer.Display();
    delay(10);
}