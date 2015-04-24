// struct for holding a data packet
typedef struct Packet_s {
	int type;
	int seq_no;
	int length;
	char data[512];
} Packet;

// struct for holding an ack packet
typedef struct Ack_s {
	int type;
	int ack_no;
} Ack;

// defines for packet sizes
#define PACKET_SIZE (sizeof(Packet))
#define ACK_SIZE (sizeof(Ack))

// size of data buffer for each program
#define BUFFER_LENGTH (8192)

// make c a little nicer to booleans
#define TRUE (1)
#define FALSE (0)
#define BOOL int

// I use this to show a loop that shouldn't end or break
// only exit() out of a FOREVER loop
#define FOREVER while(1)
