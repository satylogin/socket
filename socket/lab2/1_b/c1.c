#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 256
#define C_PORT 5501
#define S_PORT 5500

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main() 
{
	int c_sock;
	char buffer[SIZE+1];
	struct sockaddr_in c_addr, s_addr;

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket.");
	}
	printf("created.\n");

	// client addr structure
	bzero(&c_addr, sizeof(c_addr));
	c_addr.sin_family	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(c_sock, (const struct sockaddr*)&c_addr, sizeof(c_addr)) == -1) {
		error("binding.");
	}
	printf("binded\n");

	// server addr structure
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family 	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (connect(c_sock, (const struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
		error("connecting.");
	}
	printf("connected\n");

	struct node {
		char ch;
		int in;
		float fl;
	} data;

	printf("Enter a character, integer, float: ");
	scanf("%c %d %f", &data.ch, &data.in, &data.fl);
	sprintf(buffer, "%c %d %f", data.ch, data.in, data.fl);

	if (send(c_sock, (void*)&data, sizeof(struct node), 0) == -1) {
		printf("sending.");
	}

	printf("sent buffer: %s\n", buffer);

	close(c_sock);

	return 0;
}
