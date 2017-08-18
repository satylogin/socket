#include <stdio.h>
#include <sys/types.h>
#include <sys/scoket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int sock, new_sock, port_no, cli_len, tmp;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) {
		printf("No port provided\n");
		exit(1);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		printf("Error in socket creation.\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));

	port_no = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	tmp = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	
	if (tmp < 0) {
		printf("Error in bind.\n");
		exit(1);
	}

	listen(sock, 10);

	cli_len = sizeof(cli_addr);

	new_sock = accept(sock, (struct sockaddr*)&cli_addr, (unsigned int *)&cli_len);

	if (new_sock < 0) {
		printf("Error in new socket.\n");
		exit(1);
	}

	bzero(buffer, sizeof(buffer));

	int count = recv(new_sock, buffer, sizeof(buffer));

	if (count < 0) {
		printf("erro on recv.\n");
		exit(1);
	}

	printf("msg - %s\n", buffer);

	close(sock);

	return 0;
}
