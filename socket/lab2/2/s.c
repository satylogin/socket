#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 256
#define S_PORT 5500

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main() 
{
	int s_sock, c_sock, len, count;
	struct sockaddr_in s_addr, c_addr;
	char buffer[SIZE+1];

	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket.");
	}
	printf("socket created.\n");

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("binding socket.");
	}
	printf("socket binded\n");

	if (listen(s_sock, 10) == -1) {
		error("listining.");
	}
	printf("now listining.\n");

	bzero(&c_addr, sizeof(c_addr));
	if ((c_sock = accept(s_sock, (struct sockaddr*)&c_addr, &len)) == -1) {
		error("accepting.");
	}
	printf("connection accepted.\n");

	int id, x;

	if (recv(c_sock, (void*)&id, sizeof(id), 0) == -1) {
		error("recieving.");
	}

	printf("recieved id: %d\n", id);

	FILE *fp = fopen("dict.txt", "r");
	if (fp == NULL) {
		error("opening file");
	}

	while (fscanf(fp, "%d %s", &x, buffer) != EOF) {
		if (x == id) {
			break;
		}
	}
	if (x != id) {
		strcpy(buffer, "Address not found.");
	}

	if (send(c_sock, (void*)buffer, strlen(buffer), 0) == -1) {
		error("sending.");
	}

	printf("sent: %s", buffer);

	fclose(fp);
	close(s_sock);
	close(c_sock);

	return 0;
}
