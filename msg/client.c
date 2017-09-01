#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 256
#define S_PORT 5500
#define C_PORT 5501

void error(char msg[])
{
	printf("Error in %s\n", msg);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Insufficient argument\n");
		exit(0);
	}

	int c_sock;
	struct sockaddr_in s_addr, c_addr;
	char buffer[SIZE+5];

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket");
		exit(0);
	}
	printf("!!! SOCKET CREATED... FD: %d !!!\n", c_sock);

	bzero(&c_addr, sizeof(c_addr));
	c_addr.sin_family	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(c_sock, (const struct sockaddr*)&c_addr, sizeof(c_addr)) == -1) {
		error("binding");
		goto close;
	}
	printf("!!! SOCKET BINDED... !!!\n");

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (connect(c_sock, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("connecting");
		goto close;
	}
	printf("!!! CONNECTED TO SERVER... !!!\n");

	FILE *fp = NULL;
	fp = fopen(argv[1], "r");
	char ch;

	int i = 0;
	while ((ch = fgetc(fp)) != EOF) {
		buffer[i++] = ch;
		if (i == SIZE) {
			if (send(c_sock, (void*)buffer, i, 0) == -1) {
				error("sending data.");
			}
			i = 0;
		}
	}
	send(c_sock, (void*)buffer, i, 0);

	printf("!!! FILE SENT ... !!!\n");

close:
	close(c_sock);
	if (fp) {
		fclose(fp);
	}

	return 0;
}
