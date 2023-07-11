#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>         // std::thread
#include "packet.h"
#include <vector>
#include <string>
#include <mutex>
#include <iostream>
#include <ctime>
#include <chrono>
#include <time.h>


#define MYPORT "4950"	// the port users will be connecting to
#define SERVERPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 100
#define WINDOWSIZE 8
#define TIMER 0.5   // 500ms

using namespace std;

void take_input(int sockfd, int sockfdR);

void snd(int sockfd, struct addrinfo *p);
void receiver_snd(int sockfd, struct addrinfo *p);

void rcv(int sockfdR, int sockfdSend, struct addrinfo *p, socklen_t addr_len, struct sockaddr_storage their_addr);
void sender_rcv(int sockfdR, struct addrinfo *p, socklen_t addr_len, struct sockaddr_storage their_addr);

void handle_timer(int sockfd, struct addrinfo *p);