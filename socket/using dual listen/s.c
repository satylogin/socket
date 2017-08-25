#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define S_PORT 5500
#define C1_PORT 5501
#define C2_PORT 5502
#define SIZE 256

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main()
{
	int s_sock, c1_sock, c2_sock;

	int i, len, tmp, count;
	unsigned int len1, len2;

	char buffer[SIZE+1];

	struct sockaddr_in s_addr, c1_addr, c2_addr;

	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("socket creation");
	}	

	bzero(&s_addr, sizeof(s_addr));
	bzero(&c1_addr, sizeof(c1_addr));
	bzero(&c2_addr, sizeof(c2_addr));

	s_addr.sin_family 	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
		error("binding");
	}

	if (listen(s_sock, 10) < 0) {
		error("listning");
	}

	len1 = len2 = 0;
	c1_sock = accept(s_sock, (struct sockaddr*)&c1_addr, &len1);
	c2_sock = accept(s_sock, (struct sockaddr*)&c2_addr, &len2);

	if (c1_sock == -1 || c2_sock == -1) {
		error("accepting.");
	}

	count = recv(c1_sock, (void*)buffer, SIZE, 0);
	sscanf(buffer, "%d", &tmp);
	printf("read value = %d\n", tmp);
	sprintf(buffer, "%d", tmp);

	count = send(c2_sock, (void*)buffer, strlen(buffer), 0);

	return 0;
}
