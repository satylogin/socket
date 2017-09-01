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

	int c_sock, s_sock;
	struct sockaddr_in s_addr, c_addr;
	char buffer[SIZE+5];

	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket");
		exit(0);
	}
	printf("!!! SOCKET CREATED... FD: %d !!!\n", s_sock);

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("binding");
		goto close;
	}
	printf("!!! SOCKET BINDED... !!!\n");

	bzero(&c_addr, sizeof(c_addr));

	if (listen(s_sock, 10) == -1) {
		error("listening.");
		goto close;
	}
	printf("!!! SERVER IN LISTINING MODE... !!!\n");

	int len = 0, i;
	if ((c_sock = accept(s_sock, (struct sockaddr*)&c_addr, &len)) == -1) {
		error("connecting");
		goto close;
	}
	printf("!!! ACCEPTED REQUEST... !!!\n");

	FILE *fp = NULL;
	fp = fopen(argv[1], "w");

	while ((len = recv(c_sock, (void*)buffer, SIZE, 0))) {
		for (i = 0; i < len; ++i) {
			fputc(buffer[i], fp);
		}
	}
	printf("!!! FILE RECIEVED ... !!!\n");

close:
	close(c_sock);
	close(s_sock);
	if (fp) {
		fclose(fp);
	}

	return 0;
}
