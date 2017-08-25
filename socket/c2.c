#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SIZE 512
#define C_PORT 5502
#define S_PORT 5500

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main()
{
	int c_sock, tmp;
	char msg[] = "success";

	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (c_sock == -1) {
		error("creating socket.");
	}

	struct sockaddr_in c_addr;
	bzero(&c_addr, sizeof(c_addr));

	c_addr.sin_family 	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr	= INADDR_ANY;

	tmp = bind(c_sock, (const struct sockaddr *)&c_addr, sizeof(c_addr));
	if (tmp == -1) {
		error("binding.");
	}

	tmp = listen(c_sock, 10);

	if (tmp == -1) {
		error("listining.");
	}

	struct sockaddr_in s_addr;
	unsigned int len = 0;

	int s_sock = accept(c_sock, (void*)&s_addr, &len);

	if (s_sock == -1) {
		error("accepting server socket.");
	}

	char buffer[SIZE+1];

	int count = recv(s_sock, (void*)buffer, SIZE, 0);
	printf("%s\n", buffer);

	close(c_sock);
	close(s_sock);

	return 0;
}
