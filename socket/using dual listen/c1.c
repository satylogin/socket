#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define C_PORT 	5501
#define S_PORT 	5500
#define SIZE 	256

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main()
{
	int c_sock;
	
	int tmp, len, count;

	struct sockaddr_in c_addr, s_addr;
	
	char buffer[SIZE+1];

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("socket creation");
	}

	bzero(&c_addr, sizeof(c_addr));

	c_addr.sin_family 	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr 	= INADDR_ANY;

	if (bind(c_sock, (const struct sockaddr*)&c_addr, sizeof(c_addr)) < 0) {
		error("binding");
	}

	bzero(&s_addr, sizeof(s_addr));
	
	s_addr.sin_family 	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (connect(c_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
		error("connecting");
	}

	scanf("%d", &tmp);
	sprintf(buffer, "%d", tmp);

	count = send(c_sock, (void*)buffer, strlen(buffer), 0);

	printf("%d %s\n", count, buffer);

	return 0;
}
