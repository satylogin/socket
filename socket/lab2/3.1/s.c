#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 256
#define S_PORT 5500
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
	int s_sock, c_sock, len, count;
	struct sockaddr_in s_addr, c_addr;
	char buffer[SIZE+1], name[SIZE+1];


	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket.");
		goto close;
	}
	printf("socket created.\n");

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("binding socket.");
		goto close;
	}
	printf("socket binded\n");

	if (listen(s_sock, 10) == -1) {
		error("listining.");
		goto close;
	}
	printf("now listining.\n");

	bzero(&c_addr, sizeof(c_addr));
	if ((c_sock = accept(s_sock, (struct sockaddr*)&c_addr, &len)) == -1) {
		error("accepting.");
		goto close;
	}
	printf("connection accepted.\n");


	int id, x, i, j;
	char want[100];
	FILE *fp;

	while (true) {
		if ((x = recv(c_sock, (void*)buffer, SIZE, 0)) == -1) {
			error("recieving.");
		}
		buffer[x] = '\0';

		// decode
		if (buffer[0] == NAME) {
			j = 0;
			for (i = 1; buffer[i] != EXT; ++i) {
				want[j++] = buffer[i];
			}
			want[j] = '\0';
			printf("recieved name: %s\n", want);
		} else if (buffer[0] == NUM) {
			id = *(buffer + 1);
			printf("recieved number: %d\n", id);
		} else {
			printf("incorrect data.\n");
			break;
		}

		fp = fopen("dict.txt", "r");
		if (fp == NULL) {
			error("opening file");
		}

		while (fscanf(fp, "%d %s", &x, name) != EOF) {
			if (buffer[0] == NAME && strcmp(want, name) == 0) break;
			if (buffer[0] == NUM && x == id) break;
		}
		if ((buffer[0] == NUM && x != id) || (buffer[0] == NAME && strcmp(want, name) != 0)) {
			buffer[0] = NAME;
			strcpy(&buffer[1], "Address not found.");
		} else if (buffer[0] == NUM) {
			buffer[0] = NAME;
			strcpy(&buffer[1], name);
			i = strlen(buffer);
			buffer[i] = EXT;
			buffer[i+1] = '\0';
		} else if (buffer[0] == NAME) {
			buffer[0] = NUM;
			*(buffer+1) = x;
			buffer[5] = EXT;
			buffer[6] = '\0';
		}

		if (send(c_sock, (void*)buffer, strlen(buffer), 0) == -1) {
			error("sending.");
		}

		if (buffer[0] == NAME) {
			buffer[strlen(buffer)-1] = '\0';
			printf("%s\n", &buffer[1]);
		} else {
			printf("%d\n", x);
		}
	}

close:
	fclose(fp);
	close(s_sock);
	close(c_sock);

	return 0;
}
