#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Config.h" 

typedef struct struct_message {
    uint8_t knob;
    bool a;
    bool b;
    bool x;
    bool y;
    bool l;
    bool r;
    bool start;
    bool select;
} struct_message;

struct_message data;

void sendData(); 
void printDebug(); 

void setup() {
    Serial.begin(115200);
    Serial.println("Uruchamianie Arcade Sticka...");

    WiFi.mode(WIFI_STA);
    if (!esp_now_init()) {
        Serial.println("Blad: Nie mozna uruchomic ESP-NOW!");
        return; 
    }

    esp_now_peer_info_t info;
    info.channel = 0;
    info.encrypt = false;
    memcpy(info.peer_addr, MAC_ADDRESS_BASE, 6);
    esp_now_add_peer(&info);

    // Gałka
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);

    // Start, Select, Bumpery
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);

    // POPRAWIONE PINY A, B, X, Y (Muszą się zgadzać z odczytem!)
    pinMode(20, INPUT_PULLUP); // A
    pinMode(16, INPUT_PULLUP); // B  (TX)
    pinMode(17, INPUT_PULLUP); // X  (RX)
    pinMode(14, INPUT_PULLUP); // Y

    Serial.println("Gotowy do walki!");
}

void loop() {
    sendData();   
    printDebug(); 
    delay(50);    
}

void sendData() {
    // 1. ZEROWANIE GAŁKI (Na starcie cyklu postać stoi w miejscu)
    data.knob = 0b00000000;

    // 2. ODCZYT KIERUNKÓW (Używamy |= żeby móc dodawać bity - SKOSY!)
    if (!digitalRead(2)) data.knob |= 0b00000001; // Góra
    if (!digitalRead(3)) data.knob |= 0b00000010; // Dół
    if (!digitalRead(4)) data.knob |= 0b00000100; // Lewo
    if (!digitalRead(5)) data.knob |= 0b00001000; // Prawo

    // 3. ODCZYT PRZYCISKÓW (Z Twoimi nowymi pinami)
    data.start = !digitalRead(0);
    data.select = !digitalRead(1);

    data.a = !digitalRead(20);
    data.b = !digitalRead(16);
    data.x = !digitalRead(17);
    data.y = !digitalRead(14);

    data.l = !digitalRead(6);
    data.r = !digitalRead(7);

    // 4. WYSŁKA
    esp_now_send(MAC_ADDRESS_BASE, (uint8_t *)&data, sizeof(data));
}

void printDebug() {
    Serial.printf("Galka: %08b | A:%d B:%d X:%d Y:%d | L:%d R:%d | Start:%d Select:%d\n",
                  data.knob, 
                  data.a, data.b, data.x, data.y, 
                  data.l, data.r, 
                  data.start, data.select);
}