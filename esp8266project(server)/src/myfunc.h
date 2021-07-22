#ifndef MYFUNC_H
#define MYFUNC_H
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiAP.h>
#include <WiFiUdp.h>

struct ESPlist {
    char udppacket[255];
    char ip_adres[15];
    int chek_life;
    ESPlist* next;
};

ESPlist* newESPList(char packet[], char adres[], int time);

void updatePacketESPlist(ESPlist* temp, char packet[], int time);

#endif
