#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 	256
#define S_PORT	5500
#define START	1
#define END	2

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main() 
{
	int c_sock, s_sock, len, count;
	struct sockaddr_in s_addr, c_addr;
	char buffer[SIZE+1];

	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket");
	}
	puts("... socket created ...");
	
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;
	if (bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("binding.");
	}
	puts("... binding done ...");
	
	if (listen(s_sock, 10) == -1) {
		error("listining");
	}
	puts("... server in listen mode ...");
	
	bzero(&c_addr, sizeof(c_addr));
	if ((c_sock = accept(s_sock, (struct sockaddr *)&c_addr, &len)) == -1) {
		error("accepting");
	}
	puts("... request accepted ...");
	
	if ((count = recv(c_sock, (void *)buffer, SIZE, 0)) == -1) {
		error("recieving");
	}
	puts("... file name recieved ...");
	buffer[count] = '\0';
	printf("File to send: %s\n", buffer);
	
	FILE *fp = fopen(buffer, "r");
	if (fp == NULL) {
		error("opening file");
	}
	puts("file opened");

	char ch;
	len = 1;
	buffer[0] = START;
	while ((ch = fgetc(fp)) != EOF) {
		buffer[len++] = ch;
		if (len+1 == SIZE-10) {
			buffer[len++] =  END;
			if (send(c_sock, (void*)buffer, len, 0) == -1) {
				error("sending");
			}
			len = 0;
		}
	}
	if (len != 0) {
		buffer[len++] = END;
		if (send(c_sock, (void *)buffer, len, 0) == -1) {
			error("sending");
		}
	}
	puts("... file transfer complete ...");

	close(s_sock);
	close(c_sock);
	fclose(fp);

	return 0;
}
