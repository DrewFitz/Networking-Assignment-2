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

// common header includes
#include "data_types.h"

// buffer to send
// just the digits 12345678 repeated for 8191 bytes + null terminator
char buffer[BUFFER_LENGTH] = "1234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567";



#define TIMEOUT_SECONDS (3.0)

// check if it's been TIMEOUT_SECONDS since the given time point
BOOL hasTimeoutExpired(time_t timeout) {
	time_t now;
	time(&now);

	double secs = difftime(now, timeout);

	return secs < TIMEOUT_SECONDS ? FALSE : TRUE;
}

// send packets from start_chunk to end_chunk - 1, but not beyond total_chunks
// returns highest numbered chunk index that was sent
int sendPackets(int socket, struct sockaddr_in * serverAddr, 
				int start_chunk, int end_chunk, 
				int total_chunks, int chunk_size) 
{
	// init packet struct
	Packet packet;
	memset(&packet, 0, PACKET_SIZE);

	// return value
	int ret = start_chunk - 1;

	// quick exit if the beginning is beyond our total amount of chunks
	if (start_chunk > total_chunks) {
		return total_chunks;
	}

	for (int i = start_chunk; i < end_chunk; ++i) {
		// setup packet
		packet.type = 1;
		packet.seq_no = i;

		// if it is our final packet
		if (i == total_chunks) {
			// calculate odd length of final packet
			packet.length = BUFFER_LENGTH - (i*chunk_size);
		} else {
			// standard length
			packet.length = chunk_size;
		}

		// copy buffer into packet.data
		strncpy((char*)&(packet.data), (char*)&(buffer[i*chunk_size]), packet.length);

		// send packet
		int sendLen = sendto(socket, &packet, PACKET_SIZE, 0, (struct sockaddr *)serverAddr, sizeof(struct sockaddr_in));
		if (sendLen != PACKET_SIZE) {
			printf("send failed\n");
			exit(4);
		} else {
			printf("SEND PACKET %d\n", i);
		}

		// set return value to the index we just sent
		ret = i;
		if (i == total_chunks) { // quick exit for last packet
			return i;
		}
	}

	// return the highest packet index sent
	return ret;
}

// send the teardown message
void sendTeardown(int socket, struct sockaddr_in * serverAddr) {
	// initialize packet struct
	Packet packet;
	memset(&packet, 0, PACKET_SIZE);

	// setup packet to be teardown
	packet.type = 4;
	packet.length = 0;

	// send packet
	int sendLen = sendto(socket, &packet, PACKET_SIZE, 0, (struct sockaddr *)serverAddr, sizeof(struct sockaddr_in));
	if (sendLen != PACKET_SIZE) {
		printf("send failed\n");
		exit(5);
	} else {
		printf("SEND PACKET TEARDOWN\n");
	}
}

int main(int argc, char const *argv[]) {
	// initialize timeout
	time_t timeout_start;
	time(&timeout_start);

	// verify command line args
	if (argc != 5) {
		printf("usage: %s <server ip> <server port> <chunk size> <window size>\n", argv[0]);
		exit(1);
	}

	// get command line args
	const char* server_address = argv[1];
	unsigned short server_port = atoi(argv[2]);
	int chunk_size = atoi(argv[3]);
	int window_size = atoi(argv[4]);
	const char* local_port = "8009";

	// verify correct chunk size
	if (chunk_size > 512) {
		printf("chunk size too large, must be less than 512\n");
		exit(1);
	}

	// initalize tracking counters
	int base = 0;
	int highestSent = -1;

	// calc number of chunks to send
	int totalPackets = BUFFER_LENGTH / chunk_size;

	// initialize UDP socket
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		printf("error: socket failed");
		exit(2);
	}

	// bind the socket
	struct addrinfo *res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, local_port, &hints, &res);
	bind(sock, res->ai_addr, res->ai_addrlen);

	// struct for recv'ing ack packets
	Ack ack;
	memset(&ack, 0, ACK_SIZE);

	// structs for info from recv
	struct sockaddr_in fromAddr;
	unsigned int fromSize = sizeof(fromAddr);

	// structs for sending info
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(server_address);
	serverAddr.sin_port = htons(server_port);

	// count timeouts
	int timeouts = 0;

	// needed to check for data with select()
	struct fd_set fdset;
	struct timeval selecttime;

	// are we entering the teardown state for the first time?
	BOOL enteringTeardown = TRUE;

	FOREVER {
		// set up structs for select()
		FD_ZERO(&fdset);
		FD_SET(sock, &fdset);
		memset(&selecttime, 0, sizeof(struct timeval));

		// peek and see if socket has data to recv
		int numSet = select(sock+1, &fdset, NULL, NULL, &selecttime);

		// handle ack if necessary
		if (numSet != 0) { // we have data to read
			int recvLen = recvfrom(sock, &ack, ACK_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize);
			if (recvLen != ACK_SIZE) {
				// handle error
				printf("recv failed\n");
				exit(3);
			} else {
				// handle ack
				printf("-------- RECEIVE ACK %d\n", ack.ack_no);
				if (ack.type == 2 && ack.ack_no == base) {
					// move base forward by one
					base++;
					// total timeout reset
					time(&timeout_start);
					timeouts = 0;
				} else if (ack.type == 8 && ack.ack_no == 0) {
					// handle teardown
					printf("Done. Received teardown ack.\n");
					exit(0);
				} else if (ack.type != 2) {
					printf("*** UNRECOGNIZED ACK ***\n");
				} else {
					// only here if it's an ack with type = 2 and irrelevant ack_no
					// noop
				}
			}
		}

		// send data if necessary
		if (base == totalPackets + 1) { // TEARDOWN PHASE
			if (timeouts >= 10) {
				printf("sent max teardowns, exiting...\n");
				exit(0);
			} else if (enteringTeardown || hasTimeoutExpired(timeout_start)) {
				sendTeardown(sock, &serverAddr);
				time(&timeout_start);
				timeouts++;
				enteringTeardown = FALSE;
			}
		} else if (hasTimeoutExpired(timeout_start)) { // GO BACK N BLAST
			if (timeouts >= 10)
			{
				printf("failing after 10 timeouts\n");
				exit(1);
			}
			// resend window
			highestSent = sendPackets(sock, &serverAddr, 
									  base, base + window_size, 
									  totalPackets, chunk_size);
			// reset timeout
			time(&timeout_start);
			// increment timeout counter
			timeouts++;
		} else if (highestSent < base + window_size && highestSent + 1 < totalPackets) { // SEND UNSENT IN WINDOW
			// send available packets if ack was received
			highestSent = sendPackets(sock, &serverAddr, 
									  highestSent + 1, base + window_size, 
									  totalPackets, chunk_size);
		} else {
			// only here if no teardown, no timeout, and no data available to send in window
			// noop
		}

	} // FOREVER

	return 10; // don't ever get here, 10 = failed FOREVER (see data_types.h)
}