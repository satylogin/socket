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
	exit(0);
}

int main() 
{
	int c_sock;
	struct sockaddr_in c_addr, s_addr;
	char buffer[SIZE+1];

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("socket creation.");
	}
	printf("socket created.\n");

	bzero(&c_addr, sizeof(c_addr));
	bzero(&s_addr, sizeof(s_addr));

	c_addr.sin_family 	= AF_INET;
	c_addr.sin_port		= htons(C_PORT);
	c_addr.sin_addr.s_addr	= INADDR_ANY;

	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(c_sock, (const struct sockaddr*)&c_addr, sizeof(c_addr)) == -1) {
		error("Binding.");
	}
	printf("socket binded.\n");

	if (connect(c_sock, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("connecting.");
	}
	printf("connected to server.\n");

	int id;
	printf("enter the id of user: ");
	scanf("%d", &id);

	if (send(c_sock, (void*)&id, sizeof(id), 0) == -1) {
		printf("error in sending.");
	}

	printf("sent id: %d\n", id);

	int count;
	if ((count = recv(c_sock, (void*)buffer, SIZE, 0)) == -1) {
		error("recieveing.");
	}
	buffer[count] = '\0';

	printf("recieved: %s\n", buffer);

	close(c_sock);

	return 0;
}
