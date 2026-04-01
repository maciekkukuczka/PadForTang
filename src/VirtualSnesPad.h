#pragma once
#include "Pad.h"

class VirtualSnesPad
{
private:
Pad &pad;

public:
VirtualSnesPad(Pad &sharedPad, int clockPin, int latchPin, int dataPin);
    // --- PINY KOMUNIKACYJNE TANG NANO ---
    const int PIN_CLOCK; // Pomarańczowy kabel - Zegar (od Tanga)
    const int PIN_LATCH; // Żółty kabel - Zatrzask (od Tanga)
    const int PIN_DATA;  // Ciemnozielony i Ciemnoniebieski kabel - Dane (do Tanga)


    volatile uint16_t preparedState=0xFFFF;
    volatile uint16_t snesRegister=0xFFFF;
    volatile int bitIndex=0;

    void IRAM_ATTR OnLatchRising();
    void IRAM_ATTR OnClockRising();
    void UpdateState();
};
