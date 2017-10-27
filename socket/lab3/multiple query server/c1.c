#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define S_PORT	5500
#define C_PORT	5501
#define START	1
#define END	2
#define SIZE 	256
#define ETX 	3

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main(int argc, char *argv[]) 
{
	if (argc != 2) {
		puts("Invalid number of arguments, program terminating");
		exit(0);
	}

	int 	c_sock;
	struct 	sockaddr_in c_addr, s_addr;
	char 	buffer[SIZE+1];

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket.");
	}
	puts("... socket created ...");

	bzero(&c_addr, sizeof(c_addr));
	bzero(&s_addr, sizeof(s_addr));

	c_addr.sin_family 	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr	= INADDR_ANY;

	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(c_sock, (const struct sockaddr *)&c_addr, sizeof(c_addr)) == -1) {
		error("binding socket.");
	}
	puts("... socket binded ... ");

	if (connect(c_sock, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
		error("connecting.");
	}
	puts("... connected to server ...");

	sprintf(buffer, "%s", argv[1]);

	if (send(c_sock, (void*)buffer, strlen(buffer), 0) == -1) {
		error("sending");
	}
	puts("... request sent ...");

	puts("... waiting to recieve ...");
	int count, i;
	while ((count = recv(c_sock, (void*)buffer, SIZE, 0)) != -1) {
		if (count == 0) break;
		if (buffer[0] == ETX) break;
		buffer[count] = '\0';
		printf("%s", buffer);
	}
	puts("");
	puts("... done recieving ... ");

	close(c_sock);

	return 0;
}
