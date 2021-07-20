#include "myfunc.h"


ESPlist* newESPList(char packet[], char adres[], int time)
{
	ESPlist* temp = new ESPlist;
	strcpy(temp->udppacket, packet);
	strcpy(temp->ip_adres, adres);
    temp->chek_life = time;
    temp->next = NULL;

	return temp;
}

void updatePacketESPlist(ESPlist* temp, char packet[], int time) {
    temp->chek_life = time;
    strcpy(temp->udppacket, packet);
}

