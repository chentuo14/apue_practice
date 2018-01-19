#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>

#define oops(msg) do{ perror(msg); exit(1);} while(0)

int main(int argc, char *argv[])
{
	int sock_id, msglen;
	struct sockaddr_in my_addr;
	char message[BUFSIZ];

	if(argc != 3)
		oops("usage: mytimeclt hostname 43210");

	/* Step 1: Get a socket */
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_id == -1)
		oops("socket");

	/* Step 2: Connect to server */
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[2]));
	if(inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr) != 1)
		oops("inet_pton");
	
	if(connect(sock_id, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0)
		oops("connect");

	/* Step 3: Transfer data from server,then hangup */
    msglen = read(sock_id, message, BUFSIZ);
	if(msglen == -1)
		oops("read");

	if(write(1, message, msglen) != msglen)
		oops("write");

	close(sock_id);
	return 0;
}
