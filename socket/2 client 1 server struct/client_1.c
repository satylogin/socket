#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SIZE 256
#define C_PORT 5501
#define S_PORT 5500

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

struct node {
	char 	char_data;
	int 	int_data;
	float 	float_data;
};

int main()
{
	int c_sock, tmp, len;
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

	struct sockaddr_in s_addr;
	bzero(&s_addr, sizeof(s_addr));

	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;
	// inet_pton(AF_INET, ip, &s_addr.sin_addr);

	tmp = connect(c_sock, (const struct sockaddr *)&s_addr, sizeof(s_addr));

	if (tmp == -1) {
		error("connecting.");
	}

	char buffer[SIZE+1];
	printf("Enter a char, int, float:\n");
	gets(buffer);

	int count = send(c_sock, (void*)buffer, strlen(buffer), 0);

	if (strcmp(buffer, "bye") == 0) {
		printf("Exiting chatbot.\n");
	}

	close(c_sock);

	return 0;
}
