#include "VirtualNesPad.h"

VirtualNesPad::VirtualNesPad(Pad &sharedPad, int clockPin, int latchPin, int dataPin)
    : pad(sharedPad), PIN_CLOCK(clockPin), PIN_LATCH(latchPin), PIN_DATA(dataPin) {};

void IRAM_ATTR VirtualNesPad::OnLatchRising()
{

    // 3. Zapisujemy odczytany stan do naszego głównego, globalnego rejestru
    nesRegister = preparedState;
    bitIndex = 0;

    /* // 4. TAJEMNICA SPRZĘTOWA NES-a:
        // Kiedy LATCH idzie w górę, oryginalny pad natychmiast, bez czekania na ZEGAR,
        // wystawia na kabel DATA stan pierwszego przycisku (czyli bit 0 - przycisk A).
        // Musimy to zasymulować! Odczytujemy bit zerowy (bitRead) i wysyłamy na kabel. */

    // if (bitRead(nesRegister, 0) == 0)
    if (!(nesRegister & 1))
    {
        // digitalWrite(PIN_DATA, LOW); // Jeśli zero (wciśnięty), wyślij LOW
        gpio_set_level((gpio_num_t)PIN_DATA, 0);
    }
    else
    {
        // digitalWrite(PIN_DATA, HIGH); // Jeśli jedynka (puszczony), wyślij HIGH
        gpio_set_level((gpio_num_t)PIN_DATA, 1);
    }
}

void IRAM_ATTR VirtualNesPad::OnClockRising()
{
    /* if (micros() - lastClockTime < 4)
        return;
    lastClockTime = micros(); */
    // Tang Nano uderzył w zegar! Zwiększamy nasz licznik, żeby wziąć kolejny przycisk.

    // Zabezpieczenie inżynieryjne: oryginalny pad ma 8 przycisków (bity 0-7).
    // Jeśli Tang zwariuje i wyśle więcej impulsów zegara, nie chcemy wyjść poza pamięć.
    if (++bitIndex > 7)
        bitIndex = 7;
    // Odczytujemy kolejny bit z naszego zamrożonego rejestru i wystawiamy na kabel
    // if (bitRead(nesRegister, bitIndex) == 0)
    if (!(nesRegister & (1 << bitIndex)))
        // digitalWrite(PIN_DATA, LOW);
        gpio_set_level((gpio_num_t)PIN_DATA, 0);
    else
        // digitalWrite(PIN_DATA, HIGH);
        gpio_set_level((gpio_num_t)PIN_DATA, 1);
}

void VirtualNesPad::UpdateState()
{
    uint8_t currentState = 0xFF;

    // CYFROWY PIVOT (SOCD Cleaner)
    if (pad.LeftPressed() && pad.RightPressed())
    {
        // Jeśli gracz (lub błąd BT) wcisnął oba naraz, anuluj oba!
        bitSet(currentState, 6); // Zignoruj LEWO (1 = puszczony)
        bitSet(currentState, 7); // Zignoruj PRAWO (1 = puszczony)
    }
    if ((pad.UpPressed() && pad.DownPressed()))
    {
        bitSet(currentState, 4); // Zignoruj GÓRA (1 = puszczony)
        bitSet(currentState, 5); // Zignoruj DÓŁ (1 = puszczony)
    }

    /*     // FILTR ZER (Failsafe)
    if (currentState == 0x00)
    {
        currentState = 0xFF; // Przywróć bezpieczny stan (nic nie wciśnięte)
    } */

    if (pad.APressed())
        bitClear(currentState, 0);
    if (pad.BPressed())
        bitClear(currentState, 1);
    if (pad.SelectPressed())
        bitClear(currentState, 2);
    if (pad.StartPressed())
        bitClear(currentState, 3);
    if (pad.UpPressed())
        bitClear(currentState, 4);
    if (pad.DownPressed())
        bitClear(currentState, 5);
    if (pad.LeftPressed())
        bitClear(currentState, 6);
    if (pad.RightPressed())
        bitClear(currentState, 7);

    // Zapisujemy do "skrzynki pocztowej"
    preparedState = currentState;
}
