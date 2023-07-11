#include "threads.h"
vector<Packet *> inputBuf;        // store the input as packets in length payload
vector<Packet *> ackedBuf;        // store the ACKED packets
mutex mtx;
mutex mtx2;
mutex mtxExp;

int base = 0;       // base for window
int nextSeqNum = 0; // next sequence number
int expSeqNum = 0;  // expected sequence number in the receiver

chrono::steady_clock::time_point start;

// This function takes input and stores the input in inputBuf, as packets.
void take_input(int sockfd, int sockfdR)
{
    string s = "";
    string prev = "somestring";
    while(1)
    {
        string tmp = "";

        std::getline(cin, s);
        if(s == "" && prev == "")   // If the user presses enter two times, then terminate the connection.
        {
            shutdown(sockfdR, 2);       // shutdown the socket
            exit(0);
        }
        
        if(s.length() == 0)     // if it is an empty string, just send '\n' character, so that receiver can terminate the connection
        {
            char *msg = "\n";
            Packet *pckt = create_packt(0, 0, msg);    // assign ack -1 because this is not an ack
            mtx.lock();
            inputBuf.push_back(pckt);
            mtx.unlock();
        }

        // Here the input string is divided into PAYLOAD number of bytes, i.e., packets that have size PAYLAOD + metadata. (17 in this case, see readme.md)
        for(int i=0; i<s.length(); i++)
        {
            tmp += s[i];
            if(i%(PAYLOAD) == PAYLOAD-1 || i == s.length()-1)     // make packets with length 16
            {
                char *msg = new char[MAXBUFLEN];
                if(i == s.length() - 1)
                    tmp += '\n';        // Concatanate the '\n' character to each input line

                strcpy(msg, tmp.c_str());
                Packet *pckt = create_packt(0, 0, msg);    // assign ack -1 because this is not an ack, it is a data packet
                mtx.lock();
                inputBuf.push_back(pckt);
                mtx.unlock();
                tmp = "";
            }
        }
        prev = s;
    }
}

// This function sends one packet from the WINDOW in each while loop.
void snd(int sockfd, struct addrinfo *p)
{
    int numbytes;
    while(1)
    {
        /////////////////SENDER SENDS PACKETS/////////////////
        mtx.lock();
        int sz = inputBuf.size();
        mtx.unlock();
        mtx2.lock();
        bool f = sz != 0 && nextSeqNum < base + WINDOWSIZE && nextSeqNum < sz;
        mtx2.unlock();
        if(f)
        {
            // SEND the packet!
            Packet *curpack = nullptr;
            mtx.lock();
            curpack = inputBuf[nextSeqNum];
            mtx.unlock();
            if(curpack)
            {
                mtx2.lock();
                curpack->seqNumber = nextSeqNum;
                mtx2.unlock();
                if ((numbytes = sendto(sockfd, (void *) curpack, sizeof(struct Packet), 0,
                                p->ai_addr, p->ai_addrlen)) == -1) {
                    perror("talker thread: sendto");
                    exit(1);
                }
            }
            mtx2.lock();
            if(base == nextSeqNum)
            {
                int a = 1;
                start = chrono::steady_clock::now();    //start timer
            }
            nextSeqNum = (nextSeqNum+1) % INT32_MAX;        // to avoid overflow
            mtx2.unlock();
        }
    }
}


// This function receives the message and prints it out.
void rcv(int sockfdR, int sockfdSend, struct addrinfo *p, socklen_t addr_len, struct sockaddr_storage their_addr)
{
    addr_len = sizeof(their_addr);
    int numbytes;
    string prev;
    while(1)
    {
        /////////////////RECEIVER RECEIVES PACKETS/////////////////
		struct Packet *rcv_pckt = new Packet();
        if ((numbytes = recvfrom(sockfdR, (void *)rcv_pckt, (sizeof(Packet)-1) , 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}

        string msg = rcv_pckt->payload;
        if(msg == "\n")
        {
            exit(0);        // if just \n character is sent, terminate the connection
        }
        prev = msg;
        rcv_pckt->payload[strlen(rcv_pckt->payload)] = '\0';    // Make it C str

        // SEND ACK!
        mtxExp.lock();
        int expSeq = expSeqNum;
        mtxExp.unlock();

        /////////////////RECEIVER SENDS ACKS/////////////////
        // In GBN we don't have any buffering mechanism at the receiver, so immediately send the ACKS!

        // SEND ACK EXP here, if the packet we expect is arrived!
        // Increment the EXPECTED SEQUENCE NUMBER
        if(rcv_pckt->ACK != 1 && rcv_pckt->seqNumber == expSeq)// && rcv_pckt->checksum == calc_checksum(rcv_pckt->payload))
        {   
            Packet *tmp = create_packt(expSeq, 1, "ACK");
            // Send ACKS
            if ((numbytes = sendto(sockfdSend, (void *) tmp, sizeof(struct Packet), 0,
                            p->ai_addr, p->ai_addrlen)) == -1) {
                perror("talker thread: sendto");
                exit(1);
            }
            mtxExp.lock();
            expSeqNum = (expSeqNum+1) % INT32_MAX;        // to avoid overflow
            mtxExp.unlock();
            cout << rcv_pckt->payload;
        }
        // If coming packets are not EXPECTING PACKET, just ignore them by sending ACK with expSeqNum
        // Here, DON'T increment expectedseqnum!
        else if(rcv_pckt->ACK != 1)
        {
            Packet *tmp = create_packt(expSeq, 1, "ACK");

            // Send ACKS
            if ((numbytes = sendto(sockfdSend, (void *) tmp, sizeof(struct Packet), 0,
                            p->ai_addr, p->ai_addrlen)) == -1) {
                perror("talker thread: sendto");
                exit(1);
            }
        }
        /////////////////SENDER RECEIVE ACK/////////////////
        if( rcv_pckt->ACK == 1)
        {
            mtx2.lock();
            base = rcv_pckt->seqNumber + 1;
            mtx2.unlock();
            if(base != nextSeqNum)
            {
                start = chrono::steady_clock::now();    // start timer
            }
        }
    }
}

void handle_timer(int sockfd, struct addrinfo *p)
{
    int numbytes;

    // If timeout, start timer again and send all packets in the the window again 
    while(1)
    {
        chrono::steady_clock::time_point now = chrono::steady_clock::now();
        chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        // chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(now - start);
        if(time_span.count() > TIMER)
        {
            start = chrono::steady_clock::now();

            mtx2.lock();
            int bs = base;
            int nsN = nextSeqNum;
            mtx2.unlock();
            // Iterate over the window and send all window
            for(int i=bs; i<nsN; i++)
            {
                Packet *curpack = nullptr;
                mtx.lock();
                curpack = inputBuf[i];
                mtx.unlock();
                if(curpack)
                {
                    curpack->seqNumber = i;
                    // SEND
                    if ((numbytes = sendto(sockfd, (void *) curpack, sizeof(struct Packet), 0,
                                    p->ai_addr, p->ai_addrlen)) == -1) {
                        perror("talker thread: sendto");
                        exit(1);
                    }
                }
            }
        }
    }
}
