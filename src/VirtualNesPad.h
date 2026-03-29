#pragma once
#include <Arduino.h>
#include "Pad.h"

class VirtualNesPad
{
private:
    uint32_t lastClockTime;
    uint32_t lastLatchTime;
    // ZNAK '&' JEST KLUCZOWY! Oznacza: "będę tylko patrzył na cudzą pamięć"
    Pad &pad;

public:
    // Konstruktor: "Przy narodzinach musisz mi podać adres do prawdziwego Pada"
    VirtualNesPad(Pad &sharedPad, int clockPin, int latchPin, int dataPin);

    // --- PINY KOMUNIKACYJNE TANG NANO ---
    const int PIN_CLOCK; // Pomarańczowy kabel - Zegar (od Tanga)
    const int PIN_LATCH; // Żółty kabel - Zatrzask (od Tanga)
    const int PIN_DATA;  // Ciemnozielony i Ciemnoniebieski kabel - Dane (do Tanga)

    // --- NASZ WIRTUALNY REJESTR (Pamięć pada) ---
    // uint8_t to specjalna zmienna, która ma dokładnie 8 bitów (8 miejsc na zera lub jedynki).
    // Idealnie pasuje do 8 przycisków klasycznego pada!
    // Wpisujemy na start 0xFF (same jedynki: 11111111), bo przypominam:
    // w starych konsolach "1" = puszczony, "0" = wciśnięty. */
    volatile uint8_t preparedState = 0xFF; // "Skrzynka pocztowa" (przygotowana paczka)
    volatile uint8_t nesRegister = 0xFF;
    volatile uint8_t bitIndex = 0; // Licznik wysłanych bitów (od 0 do 7)
    // volatile Mówi kompilatorowi: "Ej, ta zmienna będzie modyfikowana przez przerwania sprzętowe w tle, więc nie próbuj jej optymalizować!"

    void UpdateState(); // Nowa, zwykła funkcja (NIE przerwanie!)
    void IRAM_ATTR OnLatchRising();
    void IRAM_ATTR OnClockRising();
};
