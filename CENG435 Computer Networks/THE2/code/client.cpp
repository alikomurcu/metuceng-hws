/*
** talker.c -- a datagram "client" demo
* This script is obtained from Beej's guide to network programming
* and edited by Ali Komurcu
*/
#include "threads.h"

int main(int argc, char *argv[])
{
	int sockfd, sockfdR;
	struct addrinfo hints, hintsR, *servinfo, *servinfoR, *p, *pR;
	int rv;
	int numbytes;
	char input[MAXBUFLEN];
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
    int yes=1;
	
	// To send
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	
	// Get address info
	if ((rv = getaddrinfo("172.24.0.10", SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		// Create a socket for sending
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		// Connect that socket
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("talker: connect");
			continue;
		}
		break;
	}

	// To receive
	memset(&hintsR, 0, sizeof hintsR);
	hintsR.ai_family = AF_INET; // set to AF_INET to use IPv4
	hintsR.ai_socktype = SOCK_DGRAM;
	hintsR.ai_flags = AI_PASSIVE; // use my IP

	// Get address info
	if ((rv = getaddrinfo(NULL, SERVERPORT, &hintsR, &servinfoR)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(pR = servinfoR; pR != NULL; pR = pR->ai_next) {
		// Create a socket for receiving
		if ((sockfdR = socket(pR->ai_family, pR->ai_socktype,
				pR->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		// Bind that socket
		if (bind(sockfdR, pR->ai_addr, pR->ai_addrlen) == -1)
		{
			close(sockfdR);
			perror("talker: connect");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	addr_len = sizeof their_addr;
	const char* k = "";

	std::thread t1(snd, sockfd, p);
	std::thread t2(handle_timer, sockfd, p);
	std::thread t3(rcv, sockfdR, sockfd, p, addr_len, their_addr);
	std::thread t5(take_input, sockfd, sockfdR);

	t1.join();
	t2.join();
	t3.join();
	t5.join();

	freeaddrinfo(servinfo);
	freeaddrinfo(servinfoR);

	close(sockfd);
	close(sockfdR);

	return 0;
}
