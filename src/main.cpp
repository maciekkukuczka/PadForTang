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

// --- PINY KOMUNIKACYJNE TANG NANO ---
const int PIN_CLOCK = 3;  // Pomarańczowy kabel - Zegar (od Tanga)
const int PIN_LATCH = 4;  // Żółty kabel - Zatrzask (od Tanga)
const int PIN_DATA  = 5;  // Ciemnozielony kabel - Dane (do Tanga)

// --- NASZ WIRTUALNY REJESTR (Pamięć pada) ---
// uint8_t to specjalna zmienna, która ma dokładnie 8 bitów (8 miejsc na zera lub jedynki).
// Idealnie pasuje do 8 przycisków klasycznego pada!
// Wpisujemy na start 0xFF (same jedynki: 11111111), bo przypominam: 
// w starych konsolach "1" = puszczony, "0" = wciśnięty.
volatile uint8_t nesRegister = 0xFF;

// Słówko IRAM_ATTR jest KRYTYCZNE na ESP32!
// Mówi procesorowi: "Trzymaj tę funkcję w najszybszej pamięci RAM, a nie we flashu",
// dzięki czemu przerwanie wykona się błyskawicznie i nie zawiesi układu.
void IRAM_ATTR OnLatchRising(){
    // 1. Zaczynamy z czystą kartą: 0xFF to binarnie 11111111 (wszystkie przyciski puszczone
    uint8_t currentState=0xFF;

    // 2. Jeśli dany przycisk jest wciśnięty, "zerujemy" odpowiedni bit w naszym bajcie.
    // Używamy wbudowanej funkcji bitClear(zmienna, numer_bitu)
    // Kolejność Pegasusa to ZAWSZE: A, B, Select, Start, Up, Down, Left, Right

   if (pad.APressed())      bitClear(currentState, 0); // Bit 0 to przycisk A
    if (pad.BPressed())      bitClear(currentState, 1); // Bit 1 to B
    if (pad.SelectPressed()) bitClear(currentState, 2); // Bit 2 to Select
    if (pad.StartPressed())  bitClear(currentState, 3); // Bit 3 to Start
    if (pad.UpPressed())     bitClear(currentState, 4); // Bit 4 to Up
    if (pad.DownPressed())   bitClear(currentState, 5); // Bit 5 to Down
    if (pad.LeftPressed())   bitClear(currentState, 6); // Bit 6 to Left
    if (pad.RightPressed())  bitClear(currentState, 7); // Bit 7 to Right

    // 3. Zapisujemy odczytany stan do naszego głównego, globalnego rejestru
    nesRegister = currentState;

// 4. TAJEMNICA SPRZĘTOWA NES-a:
    // Kiedy LATCH idzie w górę, oryginalny pad natychmiast, bez czekania na ZEGAR,
    // wystawia na kabel DATA stan pierwszego przycisku (czyli bit 0 - przycisk A).
    // Musimy to zasymulować! Odczytujemy bit zerowy (bitRead) i wysyłamy na kabel.

 if (bitRead(nesRegister, 0) == 0) {
        digitalWrite(PIN_DATA, LOW);  // Jeśli zero (wciśnięty), wyślij LOW
    } else {
        digitalWrite(PIN_DATA, HIGH); // Jeśli jedynka (puszczony), wyślij HIGH
    }
    

}

void setup()
{
    Serial.begin(115200);
    Wire.begin(20, 21);

// Konfiguracja kierunków (kto słucha, a kto nadaje)
    pinMode(PIN_CLOCK, INPUT);  // ESP słucha zegara
    pinMode(PIN_LATCH, INPUT);  // ESP słucha zatrzasku
    pinMode(PIN_DATA, OUTPUT);  // ESP nadaje dane
    
    // Na start ustawiamy na linii Danych stan wysoki (puszczony przycisk)
    digitalWrite(PIN_DATA, HIGH);

// Podpinamy naszą funkcję onLatchRising pod żółty kabel.
    // RISING oznacza, że funkcja odpali się dokładnie w tym ułamku mikrosekundy,
    // gdy napięcie na kablu skoczy z 0V na 3.3V.

   attachInterrupt(digitalPinToInterrupt(PIN_LATCH), OnLatchRising, RISING);
    


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
    text.ShowText("System GOTOWY!",0, 20, false);
    renderer.Display();
    delay(1000);
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
        Serial.println("Pad ROZLACZONY!");
        text.ShowText("Pad ROZLACZONY!");
    }
#endif

    renderer.Display();
    delay(10);
}