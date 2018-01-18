#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#define oops(msg) { perror(msg); exit(1);}

main(int argc, char *argv[])
{
    struct sockaddr_in servadd;
    struct hostent *hp;
    int sock_id, sock_fd;
    char message[BUFSIZ];
    int messlen;

    /* step 1: get a socket */
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == -1)
        oops("socket");
    
    /* step 2: connect to server */
    bzero(&servadd, sizeof(servadd));
    
    hp = gethostbyname(argv[1]);
    if(hp == NULL)
        oops(argv[1]);

    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(atoi(argv[2]));
    servadd.sin_family = AF_INET;

    if(connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect");
    
    /*setp 3: transfer data from server, then hangup */
    messlen = read(sock_id, message, BUFSIZ);
    if(messlen == -1)
        oops("read");
    if(write(1, message, messlen) != messlen)
        oops("write");
    close(sock_id);
}