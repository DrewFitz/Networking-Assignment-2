# Networking Programming Assignment 2 ########

Written by: Andrew Fitzpatrick - drew.fitz@uky.edu

## Build Instructions ########

Build both the client and server with:

	make all

Remove build files and debugging symbols with:

	make clean


## Running Instructions ########

Run the server (receiver)

	./myserver <listen_port> [<loss_percentage>]

Where listen_port is the port the server will listen on, and loss_percentage is an integer from 0 to 100 indicating how many packets to lose.

The server will run until it receives a teardown message from the client, then it will wait for 7 seconds, print out the received data string and exit.


Run the client (sender)

	./myclient <server_ip> <server_port> <chunk_size> <window_size>

Where:
* server_ip is the IP address of the machine that myserver is running on
* server_port is the port the myserver program is listening on
* chunk_size is the size of each data segment sent (must be less than 512)
* window_size is the size of the go*back*N window

The client will run until it does 10 timeouts in a row without a new ACK or until it receives ACKs for every packet sent. Then it will send a teardown message up to 10 times or until it receives the special teardown ACK.

## Caveats and Limitations ########

The data sent is a constant buffer of 8192 bytes which is the string '12345678' repeated with the final byte replaced with a null terminator.

Not tested for and probably will not correctly handle multiple transfers simultaneously.

The client is hard-coded to listen for ACKs on port 8009.