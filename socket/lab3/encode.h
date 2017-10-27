#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef UNISTD_H
#define UNISTD_H
#include <unistd.h>
#endif

#ifndef SYS_TYPES_H
#define SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifndef NETINET_IN_H
#define NETINET_IN_H
#include <netinet/in.h>
#endif

#ifndef ENCODE_DECODE
#define ENCODE_DECODE

#define START 		1
#define END		2
#define PAUSE		3
#define CONT		4
#define BUFFER_SIZE	512
#define MSG_SIZE	512

// return number of bytes put in buffer
int encode(char buffer[], char msg[], int len, int start_type, int end_type)
{
	// mark the start
	buffer[0] = start_type;
	
	// fill the buffer
	int i;
	for (i = 1; i <= len && i < BUFFER_SIZE; ++i) {
		buffer[i] = msg[i-1];
	}

	// mark the end
	buffer[i] = end_type;
	
	// return number of bytes
	return i-1;
}

// returns the number of bytes returned
int decode(char buffer[], char msg[], int len)
{
	// read the buffer till a terminator
	int i;
	for (i = 1; i <= len && buffer[i] != END && buffer[i] != PAUSE; ++i) {
		msg[i-1] = buffer[i];
	}

	// return the number of bytes read
	return i;
}

#endif
