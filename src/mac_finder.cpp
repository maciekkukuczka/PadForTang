#include <Arduino.h>
#include <WiFi.h>

void setup(){
Serial.begin(115200);
delay(2000);
WiFi.mode(WIFI_STA);

Serial.println("Adres Mac:  " + WiFi.macAddress());
}

void loop(){}