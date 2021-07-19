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

ESPlist* newESPList(char packet[], char adres[], int time)
{
	ESPlist* temp = new ESPlist; //выделение памяти
	strcpy(temp->udppacket, packet);
	strcpy(temp->ip_adres, adres);
    temp->chek_life = time;
    temp->next = NULL;

	return temp; // возвращение адреса элемента
}

void updatePacketESPlist(ESPlist* temp, char packet[], int time) {
    temp->chek_life = time;
    strcpy(temp->udppacket, packet);
}

void changeESPlistPlace(ESPlist* tempESP,ESPlist* HeadESPlist, ESPlist* preview) {
    ESPlist* temp = HeadESPlist->next;
    preview->next = tempESP->next;
    tempESP->next = temp;
    HeadESPlist = tempESP;
}

