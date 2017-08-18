#include <stdio.h>
#include <sys/types.h>
#include <sys/scoket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 5501
#define MAX_LINE 256

int main(int argc, char *argv[])
{
	char *msg;

	if (argc == 2) {
		msg = argv[1];
	} else {
		printf("Can send only one work at a time.\n");
		exit(1);
	}



	return 0;
}
