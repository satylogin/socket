#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define S_PORT	5500
#define C_PORT	5502
#define START	1
#define END	2
#define SIZE 256

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main(int argc, char *argv[]) 
{
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

	if (fork() == 0) {
		int count, i;
		char msg[SIZE+1];
		while ((count = recv(c_sock, (void *)msg, SIZE, 0)) != -1) {
			if (count == 0) break;
			msg[count] = '\0';
			printf("server: %s\n", msg);
		}
	} else {
		int count, i;
		char msg[SIZE+1];
		while (1) {
			gets(msg);
			while (send(c_sock, (void *)msg, strlen(msg), 0) == -1);
			if (strcmp(msg, "exit") == 0) break;
		}
	}

	close(c_sock);

	return 0;
}
