#include "packet.h"

// Calculate the checksum
int calc_checksum(char payload[PAYLOAD])
{
    int halfsz = PAYLOAD/2;
    int res = 0;
    for(int i=0; i<halfsz; i++)
    {
        res += payload[i] + payload[i+halfsz];
    }
    return res;
}

// Create a packet with this function
Packet* create_packt(int seqNumber, char ACK, char payload[])
{
    Packet *pckt = new Packet();
    pckt->seqNumber = seqNumber;
    pckt->ACK = ACK;
    strcpy(pckt->payload, payload);
    pckt->checksum = calc_checksum(payload);
    return pckt;
}