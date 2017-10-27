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

	if (fork() == 0) {
		int count, i;
		char msg[SIZE+1];
		while ((count = recv(c_sock, (void *)msg, SIZE, 0)) != -1) {
			msg[count] = '\0';
			printf("user: %s\n", msg);
		}
		exit(0);
	} else {
		char msg[SIZE+1];
		int count, i;
		while (1) {
			gets(msg);
			while (send(c_sock, (void *)msg, strlen(msg), 0) == -1);
			if (strcmp(msg, "exit") == 0) break;
		}
	}

	close(s_sock);
	close(c_sock);

	return 0;
}
