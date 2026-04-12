#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Config.h"

typedef struct struct_message
{
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

void setup()
{
    WiFi.mode(WIFI_STA);
    if (!esp_now_init())
        return;
    esp_now_peer_info_t info;
    info.channel=0;
    info.encrypt=false;
    memcpy(info.peer_addr, MAC_ADDRESS_BASE, 6);
    esp_now_add_peer(&info);

    pinMode(0, INPUT);
    pinMode(1, INPUT);


}

void loop()
{
    send(data);
    delay(10);
}

void send(struct_message data)
{
    if (digitalRead(2))
    {
        data.knob = 0b00000001;
    }
    else if (digitalRead(3))
    {
        data.knob = 0b00000010;
    }
    else if (digitalRead(4))
    {
        data.knob = 0b00000100;
    }
    else if (digitalRead(5))
    {
        data.knob = 0b00001000;
    }
    else
    {
        data.knob = 0b00000000;
    }

    data.start = digitalRead(0);
    data.select = digitalRead(1);

    data.a = digitalRead(6);
    data.b = digitalRead(7);
    data.x = digitalRead(10);
    data.y = digitalRead(11);
    data.l = digitalRead(12);
    data.r = digitalRead(13);

    esp_now_send(MAC_ADDRESS_BASE, (uint8_t *)&data, sizeof(data));
};
