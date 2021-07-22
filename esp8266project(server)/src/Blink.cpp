#include "myfunc.h"
ESPlist* HeadESPlist = NULL;

inline void handleClient(WiFiClient& client, ESPlist* list);

const char* ssid = "esp8266";
const char* password = "12345611";

WiFiServer server(80);

WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[255];
char replyPacekt[] = "Пакет принят!!!";

void setup()
{
  Serial.begin(9600);
  Serial.println();
  
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop()
{
  WiFiClient client = server.available();
    if (client) {
        handleClient(client, HeadESPlist);
  }

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    char ip_adres[15];
    strcpy(ip_adres, Udp.remoteIP().toString().c_str());
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, ip_adres, Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacekt);
    Udp.endPacket();

    if (HeadESPlist == NULL) {
      int time = millis() / 1000;
      HeadESPlist = newESPList(incomingPacket, ip_adres, time);
    }

    ESPlist* tempESP = HeadESPlist;
    ESPlist* previous = NULL;

    while (tempESP != NULL) {

      String adresTemp = ip_adres; //из пакета
      String adresFromStruct = tempESP->ip_adres; //из текущего элемента

      if (adresTemp == adresFromStruct) {

        int time = millis() / 1000;
        updatePacketESPlist(tempESP, incomingPacket, time);

        if (tempESP != HeadESPlist) {
        previous->next = tempESP->next;
        tempESP->next = HeadESPlist;
        HeadESPlist = tempESP;
        }
        
        break;
      }
      if (tempESP->next == NULL) {
        int time = millis() / 1000;
        ESPlist* head = HeadESPlist;
        HeadESPlist = newESPList(incomingPacket, ip_adres, time);
        HeadESPlist->next = head;
        break;
      } else {
        previous = tempESP;
        tempESP = tempESP->next;
      }
    }
    if (HeadESPlist != NULL) {
      ESPlist* tempESP = HeadESPlist;
      int i = 0;
      while(tempESP != NULL) {
      Serial.print("Элемент № "); Serial.println(i);
      Serial.print("Адрес элемента списка = "); Serial.println(tempESP->ip_adres);
      Serial.print("Пакет элемента списка = "); Serial.println(tempESP->udppacket);
      int time = ((millis()/1000) - tempESP->chek_life);
      Serial.print("Времени с последнего пакета = "); Serial.println(time);
      i++;
      tempESP = tempESP->next;
      }
    }
  }



}


inline void handleClient(WiFiClient& client, ESPlist* list)
{
    Serial.println("Новый клиент.");
    while (client.connected()) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println();
      client.print("<html><meta charset=\"UTF-8\"><body>");
      
      while (list != NULL) {
        client.print("<div style = 'background: gray;'><h1>");
        client.print(list->ip_adres);
        client.print("<br>");
        client.print(list->udppacket);
        client.print("<br>");
        client.print("Последний пакет : ");
        client.print(((millis()/1000) - list->chek_life));
        client.print(" секунд назад</h1></div>");
        list = list->next;
      }
      client.print("</body></html>");
      client.println();
      break;
    }
    client.stop();
    Serial.println("Клиент отключён.");
}


