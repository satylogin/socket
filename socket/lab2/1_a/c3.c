#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 256
#define S_PORT 5500
#define C_PORT 5502

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main() 
{
	int s_sock, c_sock, m_sock;
	struct sockaddr_in s_addr, c_addr;
	char buffer[SIZE+1];

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket.");
	}

	// struct for client addr
	bzero(&c_addr, sizeof(c_addr));
	c_addr.sin_family	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(c_sock, (const struct sockaddr*)&c_addr, sizeof(c_addr)) == -1) {
		error("Binding");
	}

	bzero(&s_addr, sizeof(s_addr));

	if (listen(c_sock, 10) == -1) {
		error("listining.");
	}

	unsigned int len = 0;
	if ((m_sock = accept(c_sock, (struct sockaddr*)&s_addr, &len)) == -1) {
		error("accepting.");
	}

	int i, count;
	if ((count = recv(m_sock, buffer, SIZE, 0)) == -1) {
		error("recieveing.");
	}

	for (i = 0; i < count; ++i) {
		putchar(buffer[i]);
	}
	puts("");

	return 0;
}
