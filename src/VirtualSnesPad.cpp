#include "VirtualSnesPad.h"

VirtualSnesPad::VirtualSnesPad(Pad &sharedPad, int clockPin, int latchPin, int dataPin) : 
    pad(sharedPad), PIN_CLOCK(clockPin), PIN_LATCH(latchPin), PIN_DATA(dataPin) {};

void IRAM_ATTR VirtualSnesPad::OnLatchRising()
{
    // 1. Kopiujemy gotowy stan z wolnej pętli do szybkiego rejestru roboczego
    snesRegister = preparedState;
    
    // 2. Zerujemy licznik bitów, bo zaczynamy nową klatkę
    bitIndex = 0;

    // 3. Wystawiamy pierwszy bit (indeks 0) od razu na pin danych!
    // Używamy naszej znanej "ruchomej maski", żeby sprawdzić, czy zerowy bit to 1, czy 0
    int bitValue = (snesRegister & (1 << bitIndex)) ? 1 : 0;
    gpio_set_level((gpio_num_t)PIN_DATA, bitValue);
}

void IRAM_ATTR VirtualSnesPad::OnClockRising()
{
    // 1. Zwiększamy indeks o 1 przy każdym uderzeniu zegara
    bitIndex++;

    // 2. SNES ma 16 bitów. Pilnujemy, by nie wyjść poza zakres
    if (bitIndex < 16) 
    {
        // 3. Odczytujemy stan konkretnego bitu z rejestru i wystawiamy na pin
        int bitValue = (snesRegister & (1 << bitIndex)) ? 1 : 0;
        gpio_set_level((gpio_num_t)PIN_DATA, bitValue);
    }
}

void VirtualSnesPad::UpdateState()
{
    // Zaczynamy budowę nowego stanu. 
    // 0xFFFF oznacza binarnie 1111111111111111 (wszystkie przyciski puszczone)
    uint16_t newState = 0xFFFF;

    // --- PROTOKÓŁ SNES (Kolejność bitów) ---
    // 0: B, 1: Y, 2: Select, 3: Start, 4: Up, 5: Down, 6: Left, 7: Right, 
    // 8: A, 9: X, 10: L, 11: R, 12-15: (Nieużywane/Zawsze 1)
    
    // Uwaga: Poniżej dopasowujemy nazwy przycisków z pada Xbox do układu SNES.
    // Operacja "newState &= ~(1 << X)" to sprytny trik: wstawia '0' dokładnie na X-tej pozycji!
    
    if (pad.APressed())      newState &= ~(1 << 0);  // Xbox A -> SNES B (Bit 0)
    if (pad.XPressed())      newState &= ~(1 << 1);  // Xbox X -> SNES Y (Bit 1)
    if (pad.SelectPressed()) newState &= ~(1 << 2);  // Select (Bit 2)
    if (pad.StartPressed())  newState &= ~(1 << 3);  // Start (Bit 3)
    if (pad.UpPressed())     newState &= ~(1 << 4);  // D-pad Góra (Bit 4)
    if (pad.DownPressed())   newState &= ~(1 << 5);  // D-pad Dół (Bit 5)
    if (pad.LeftPressed())   newState &= ~(1 << 6);  // D-pad Lewo (Bit 6)
    if (pad.RightPressed())  newState &= ~(1 << 7);  // D-pad Prawo (Bit 7)
    if (pad.BPressed())      newState &= ~(1 << 8);  // Xbox B -> SNES A (Bit 8)
    if (pad.YPressed())      newState &= ~(1 << 9);  // Xbox Y -> SNES X (Bit 9)
    
    // Zależnie od tego, jak nazwałeś triggery w Pad.h:
    if (pad.L1Pressed())     newState &= ~(1 << 10); // L (Bit 10)
    if (pad.R1Pressed())     newState &= ~(1 << 11); // R (Bit 11)

    // Na koniec nadpisujemy stan zapasowy.
    // Kiedy FPGA uderzy sygnałem Latch, przerwanie OnLatchRising błyskawicznie to skopiuje.
    preparedState = newState;
}