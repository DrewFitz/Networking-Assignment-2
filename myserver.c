#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

#include "data_types.h"

char buffer[BUFFER_LENGTH];

#define TRUE (1)
#define FALSE (0)
#define BOOL int
#define FOREVER while(1)

#define TIMEOUT_SECONDS (7.0)

// check if it's been TIMEOUT_SECONDS since the given time point
BOOL hasTimeoutExpired(time_t timeout) {
	time_t now;
	time(&now);

	double secs = difftime(now, timeout);

	return secs < TIMEOUT_SECONDS ? FALSE : TRUE;
}

// for testing packet drops
// returns true if we should disregard the packet, false to keep it
BOOL should_lose_packet(double loss_rate) {
	double rv;
	rv = drand48();
	if (rv < loss_rate) return TRUE;

	return FALSE;
}

int main(int argc, char const *argv[])
{
	// seed the random number generator
	srand48(1523);

	Packet packet; 	// struct for holding received data packet
	Ack ack;		// struct for building ack to send

	time_t timeout_start; // when the timeout was last reset

	int packet_received = -1; // counter for packet receiving
	char* write_ptr = buffer; // pointer for writing packet data to local buffer

	// zero out buffer for receiving data
	memset(buffer, 0, sizeof(buffer));
	
	// check for correct command line args
	if (argc < 2 || argc > 3) {
		printf("usage: %s <listen port> [<loss percentage>]\n", argv[0]);
		printf("loss percentage should be an integer in [0, 100]\n");
		exit(1);
	}

	const char* port = argv[1];

	// get loss rate from args, default to zero loss
	double loss_rate = 0.0;
	if (argc == 3) {
		int numerator = atoi(argv[2]);
		if (numerator >= 0 && numerator <= 100) {
			loss_rate = ((double)atoi(argv[2])) / 100.0;
			printf("loss rate: %.1f%%\n", 100.0f * (float)loss_rate);
		} else {
			printf("invalid loss rate\n");
			printf("default loss rate: 0.0\n");
		}
	} else {
		printf("default loss rate: 0.0\n");
	}

	// setup socket descriptor
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0)
	{
		printf("error: socket failed");
		exit(2);
	}
	
	// bind socket to port
	struct addrinfo *res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);

	hints.ai_family 	= AF_UNSPEC;
	hints.ai_socktype 	= SOCK_DGRAM;
	hints.ai_flags 		= AI_PASSIVE;
	getaddrinfo(NULL, port, &hints, &res);
	bind(sock, res->ai_addr, res->ai_addrlen);

	// sockaddr struct for holding info about recv'd data
	unsigned int fromSize;
	struct sockaddr_in fromAddr;
	fromSize = sizeof(fromAddr);

	// are we in teardown mode?
	BOOL teardown_mode = FALSE;

	// for checking if data is ready to recv with select()
	struct fd_set fdset;
	struct timeval selecttime;

	FOREVER {
		// check if our socket has data to read
		FD_ZERO(&fdset);
		FD_SET(sock, &fdset);
		memset(&selecttime, 0, sizeof(struct timeval));
		int numSet = select(sock+1, &fdset, NULL, NULL, &selecttime);

		if (numSet != 0) { // socket is ready to read
			int recvlen = recvfrom(sock, &packet, PACKET_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize);
			if (recvlen != PACKET_SIZE) {
				printf("recv failed\n");
				exit(2);
			} 

			if (should_lose_packet(loss_rate))
			{
				printf("DROPPING PACKET %d\n", packet.seq_no);
				continue;
			}

			printf("RECEIVE PACKET %d\n", packet.seq_no);

			if (teardown_mode) { // if in teardown mode
				// respond to all packets with teardown ack
				ack.type = 8;
				ack.ack_no = 0;
			} else if (packet.type == 1) { // received data packet
				if (packet.seq_no == packet_received + 1) { // received the next packet we want
					packet_received++; // now we want the next packet

					// process this packet
					strncpy((char*)write_ptr, (char*)&packet.data, packet.length);
					write_ptr += packet.length;

					// send ack for this packet
					ack.type = 2;
					ack.ack_no = packet_received;
				} else { // received other data packet
					// send duplicate ack
					ack.type = 2;
					ack.ack_no = packet_received;
				}
			} else if (packet.type == 4 && packet.length == 0) { // received teardown
				// send teardown
				ack.type = 8;
				ack.ack_no = 0;
				// enter teardown mode with timeout
				teardown_mode = TRUE;
				time(&timeout_start);
			} else {
				printf("*** unrecognized packet ***\n");
				continue; // ignore any other packets
			}

			int sendlen = sendto(sock, &ack, ACK_SIZE, 0, (struct sockaddr *) &fromAddr, fromSize);
			if (sendlen != ACK_SIZE) {
				printf("send failed\n");
				exit(3);
			} else {
				if (ack.type == 2) {
					printf("-------- SEND ACK %d\n", ack.ack_no);
				} else if (ack.type == 8) {
					printf("-------- SEND TEARDOWN ACK\n");
				}
			}
		} else { // socket isn't ready to read
			if (teardown_mode) { // if we are tearing down
				if (hasTimeoutExpired(timeout_start)) { // check if teardown is over
					printf("---- final string ----\n");
					printf("%s\n", buffer);
					printf("---- final string ----\n");
					printf("timeout exit\n");
					exit(0);
				}
			}
		}
	}

	return 10; // don't ever get here, 10 = failed FOREVER (see data_types.h)
}