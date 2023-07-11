#include <string.h>
#include <iostream>

#define PAYLOAD 8

// The sizeof packet is 4 + 4 + 1 = 9 for metadata and 8 for payload data
// Total size of the packet is 17 bytes
// When 
struct Packet
{
    int seqNumber;
    int checksum;
    char ACK;
    char payload[PAYLOAD];   // paload of a packet
};

int calc_checksum(char *payload);

Packet* create_packt(int seqNumber,char ACK, char *payload);