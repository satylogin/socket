#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main()
{
	int c_sock;
	int c_tmp;

	char msg[] = "success";

	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (c_sock == -1) {
		printf("error in creating socket.\n");
		exit(1);
	}

	struct sockaddr_in c_myaddr;
	bzero(&c_myaddr, sizeof(c_myaddr));

	c_myaddr.sin_family = AF_INET;
	c_myaddr.sin_port = htons(5501);
	c_myaddr.sin_addr.s_addr = INADDR_ANY;

	c_tmp = bind(c_sock, (const struct sockaddr*)&c_myaddr, sizeof(c_myaddr));
	
	if (c_tmp == -1) {
		printf("error in bind.\n");
		exit(1);
	}

	char s[] = "172.19.14.103";

	struct sockaddr_in s_addr;
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(5500);
	inet_pton(AF_INET, s, &s_addr.sin_addr);
	//s_addr.sin_addr.s_addr = INADDR_ANY;

	c_tmp = connect(c_sock, (struct sockaddr*)&s_addr, sizeof(s_addr));

	if (c_tmp == -1) {
		printf("error in connect.\n");
		exit(1);
	}

	char buffer[BUFFER_SIZE+1];

	while (1) {
		printf("Enter the msg: ");
		scanf("%s", buffer);
		int count = send(c_sock, (void*)buffer, sizeof(buffer), 0);

		if (strcmp(buffer, "bye") == 0) {
			printf("Exiting chatbot.\n");
			break;
		}
	}
	close(c_sock);

	return 0;
}
