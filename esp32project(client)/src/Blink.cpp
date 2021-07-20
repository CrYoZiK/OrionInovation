#include <Arduino.h>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>
//----Temperature and Humidity---------
#include <DHT.h>
#include <DHT_U.h>

const int DHTpin = 18;
const int DHTtype = 11;
DHT dht(DHTpin, DHTtype);
//-------------------------------------
BluetoothSerial BLEesp;
//----Wifi settings--------------------
const char *ssid = "esp8266";
const char *password = "12345611";
char  replyPacekt[255];
char incomingPacket[255];


WiFiUDP Udp;
unsigned int localUdpPort = 4210;
const char *IPadress = "192.168.4.1";

void setup()
{
  Serial.begin(115200);
  BLEesp.begin("ESP32_NEW");
  Serial.println();

  dht.begin();

  Serial.printf("Подключение к %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Подключено");

  Udp.begin(localUdpPort);

}

void loop()
{
    int t = dht.readTemperature();
    int h = dht.readHumidity();
    String reply = "Температура = "+  String(t) +  "*C; Влажность = " + String(h) + " процента";
    strcpy(replyPacekt, reply.c_str());
    Serial.println(replyPacekt);
    BLEesp.println(replyPacekt);

    if (WiFi.status() == WL_CONNECTED) {
    Udp.beginPacket(IPadress, localUdpPort);
    Udp.printf(replyPacekt);
    Udp.endPacket();
    } else {
      setup();
    }

    delay(1000);
    int packetSize = Udp.parsePacket();
    
    if (packetSize > 0)
    {
      Serial.print("Есть ответный пакет :");
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.println(incomingPacket);
      
    }
    delay(2000);
}