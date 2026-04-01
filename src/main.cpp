#include <Arduino.h>
#include <Wire.h> // Biblioteka do obsługi naszej "rury z danymi" - I2C
#include "Config.h"
#include "Renderer.h"
#include "Text.h"
#include "VirtualNesPad.h"
#include "VirtualSnesPad.h"
#if USE_XBOX_PAD
#include "Pad.h"
Pad pad;
bool lastConnected = false;

VirtualNesPad nesPadPlayer1(pad, 3, 4, 5); // 1P
// VirtualNesPad nesPadPlayer2(pad,14,15,18); //2P
VirtualSnesPad snesPadPlayer1(pad, 3, 4, 5); // 1P
// VirtualNesPad nesPadPlayer2(pad,14,15,18); //2P
#endif
Renderer renderer;
Text text(renderer);
volatile int pulseCount = 0; // volatile - nie optymalizuj! zawsze odczytaj z ram
volatile bool isSnes = false;
volatile bool isAfterFirstCourse = false;

// --- FUNKCJE POŚREDNICZĄCE DLA PRZERWAŃ ---
// To one są podpinane pod piny, a same tylko "przekazują piłkę" do klasy.

// 1P
void IRAM_ATTR nesLatchWrapper1()
{
    nesPadPlayer1.OnLatchRising();
}
void IRAM_ATTR nesClockWrapper1()
{
    nesPadPlayer1.OnClockRising();
}
// 2P
/* void IRAM_ATTR latchWrapper2(){
    nesPadPlayer2.OnLatchRising();
}
void IRAM_ATTR clockWrapper2(){
    nesPadPlayer2.OnClockRising();
} */

void IRAM_ATTR snesLatchWrapper1()
{
    snesPadPlayer1.OnLatchRising();
}

void IRAM_ATTR snesClockWrapper1()
{
    snesPadPlayer1.OnClockRising();
}

void IRAM_ATTR OnLatch()
{
    if (isAfterFirstCourse)
    {

        if (pulseCount < 7)
        {
            // nesClockWrapper1();
            isSnes = false;
            detachInterrupt(3);
            detachInterrupt(4);
            attachInterrupt(digitalPinToInterrupt(nesPadPlayer1.PIN_LATCH), nesLatchWrapper1, RISING);
            attachInterrupt(digitalPinToInterrupt(nesPadPlayer1.PIN_CLOCK), nesClockWrapper1, RISING);
        }
        else
        {
            // snesClockWrapper1();
            isSnes = true;
            detachInterrupt(3);
            detachInterrupt(4);
            attachInterrupt(digitalPinToInterrupt(snesPadPlayer1.PIN_LATCH), snesLatchWrapper1, RISING);
            attachInterrupt(digitalPinToInterrupt(snesPadPlayer1.PIN_CLOCK), snesClockWrapper1, RISING);
        }
        pulseCount = 0;
    }

    isAfterFirstCourse = true;
}
void IRAM_ATTR OnClock()
{
    ++pulseCount;
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(20, 21);

    // Konfiguracja kierunków (kto słucha, a kto nadaje)
    // 1P
    pinMode(nesPadPlayer1.PIN_CLOCK, INPUT); // ESP słucha zegara
    pinMode(nesPadPlayer1.PIN_LATCH, INPUT); // ESP słucha zatrzasku
    pinMode(nesPadPlayer1.PIN_DATA, OUTPUT); // ESP nadaje dane

    // 2P
    /*     pinMode(nesPadPlayer2.PIN_CLOCK, INPUT); // ESP słucha zegara
        pinMode(nesPadPlayer2.PIN_LATCH, INPUT); // ESP słucha zatrzasku
        pinMode(nesPadPlayer2.PIN_DATA, OUTPUT); */

    // Na start ustawiamy na linii Danych stan wysoki (puszczony przycisk)
    // digitalWrite(nesPadPlayer1.PIN_DATA, HIGH);

    gpio_set_level((gpio_num_t)nesPadPlayer1.PIN_DATA, 1); // 1P

    // digitalWrite(nesPadPlayer2.PIN_DATA, HIGH);
    // gpio_set_level((gpio_num_t)nesPadPlayer2.PIN_DATA, 1); //2P

    // Podpinamy naszą funkcję onLatchRising pod żółty kabel.
    // RISING oznacza, że funkcja odpali się dokładnie w tym ułamku mikrosekundy,
    // gdy napięcie na kablu skoczy z 0V na 3.3V.

    attachInterrupt(digitalPinToInterrupt(4), OnLatch, RISING);
    attachInterrupt(digitalPinToInterrupt(3), OnClock, RISING);
    // 1P
    // attachInterrupt(digitalPinToInterrupt(nesPadPlayer1.PIN_LATCH), nesLatchWrapper1, RISING);
    // Podpinamy funkcję zegara pod pomarańczowy kabel
    // attachInterrupt(digitalPinToInterrupt(nesPadPlayer1.PIN_CLOCK), nesClockWrapper1, RISING);
    // 2P
    /*  attachInterrupt(digitalPinToInterrupt(nesPadPlayer2.PIN_LATCH), latchWrapper2, RISING);
     attachInterrupt(digitalPinToInterrupt(nesPadPlayer2.PIN_CLOCK), clockWrapper2, RISING); */

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

    if (!isSnes)
    {
        nesPadPlayer1.UpdateState(); // 1P
        // nesPadPlayer2.UpdateState(); //2P
    }
    else
    {
        snesPadPlayer1.UpdateState();
    }

    if (pad.Connected() && lastConnected == false)
    {
        lastConnected = true;
        Serial.println("Pad POLACZONY!");
        text.ShowText("Pad POLACZONY!");
        renderer.Display();

        /*  if (pad.UpPressed())
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
         } */
    }
    else if (pad.Connected() == false && lastConnected == true)
    {
        Serial.println("Pad ROZLACZONY!");
        text.ShowText("Pad ROZLACZONY!");
        lastConnected = false;
        renderer.Display();
    }
#endif

    delay(2);
}