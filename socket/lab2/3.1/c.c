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
#define NAME 1
#define NUM 9
#define EXT 3
#define true 1

void error(char msg[])
{
	printf("Error in %s\n", msg);
}

int main() 
{
	int c_sock;
	struct sockaddr_in c_addr, s_addr;
	char buffer[SIZE+1];

	if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("socket creation.");
		goto close;
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
		goto close;
	}
	printf("socket binded.\n");

	if (connect(c_sock, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("connecting.");
		goto close;
	}
	printf("connected to server.\n");

	int id, op, i, j, count;
	char name[100];

	while (true) {
		printf("enter 1 for id and 2 for name: ");
		scanf("%d", &op);

		// encode
		if (op == 1) {
			buffer[0] = NUM;
			printf("enter the id of user: ");
			scanf("%d", &id);
			*(buffer+1) = id;
			buffer[5] = EXT;
			buffer[6] = '\0';
		} else {
			buffer[0] = NAME;
			printf("enter the name of user: ");
			scanf("%s", name);
			strcpy(&buffer[1], name);
			i = strlen(buffer);
			buffer[i] = EXT;
			buffer[i+1] = '\0';
		} else {
			break;
		}

		if (send(c_sock, (void*)buffer, strlen(buffer), 0) == -1) {
			printf("error in sending.");
			continue;
		}

		if (op == 1) { 
			printf("sent id: %d\n", id);
		} else if (op == 2) {
			printf("sent name: %s\n", name);
		} else {
			printf("incorrect op\n");
			continue;
		}

		if ((count = recv(c_sock, (void*)buffer, SIZE, 0)) == -1) {
			error("recieveing.");
			continue;
		}
		buffer[count] = '\0';

		// decode
		if (buffer[0] == NAME) {
			j = 0;
			for (i = 1; buffer[i] != EXT; ++i) {
				name[j++] = buffer[i];
			}
			name[j++] = '\0';
			printf("name: %s\n", name);
		} else if (buffer[0] == NUM) {
			j = *(buffer + 1);
			printf("id: %d\n", j);
		} else {
			printf("incorrect data recieved.");
			break;
		}
	}

close:
	close(c_sock);

	return 0;
}
