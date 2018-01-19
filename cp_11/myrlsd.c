#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>

#define PORTNUM 43210
#define oops(msg) do{ perror(msg); exit(1);} while(0);

int main(int argc, char *argv[])
{
	int sock_id, sock_fd;
	int c;
	FILE *sock_fpi, *sock_fpo;
	FILE *pipe_fp;
	char dirname[BUFSIZ], command[BUFSIZ];
	struct sockaddr_in my_addr;
	/* Step 1: ask kernel for a socket */
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_id == -1)
		oops("socket");

	/* Step 2: bind address to socket. Address is host,port */
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORTNUM);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock_id, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0)
		oops("bind");

	/* Step 3: allow incoming calls with Qsize=1 on socket */
	if(listen(sock_id, 1) != 0)
		oops("listen");

	/* main loop: accept(), write(), close() */
	while(1) {
		sock_fd = accept(sock_id, NULL, NULL);
		if(sock_fd == -1)
			oops("accept");
		/* open reading direction as buffered stream */
		if((sock_fpi = fdopen(sock_fd, "r")) == NULL)
			oops("fdopen reading");

		if(fgets(dirname, BUFSIZ-5, sock_fpi) == NULL)
			oops("reading dirname");
		sanitize(dirname);

		/* open writing direction as buffered stream */
		if((sock_fpo = fdopen(sock_fd, "w")) == NULL)
			oops("fdopen writing");
		sprintf(command, "ls %s", dirname);

		if((pipe_fp = popen(command, "r")) == NULL)
			oops("popen");

		/* transfer data from ls to socket */
		while((c = getc(pipe_fp)) != EOF)
			putc(c, sock_fpo);
		pclose(pipe_fp);
		fclose(sock_fpo);
		fclose(sock_fpi);
	}
}

/* it would be very bad if someone passed us an dirname like "; rm *" */
sanitize(char *str)
{
	char *src, *dest;
	for(src=dest=str;*src;src++)
		if(*src=='/'||isalnum(*src))
			*dest++ = *src;
	*dest = '\0';
}
