#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXSIZE 10

int main(int argc, char *argv[])
{
    int fd;
    char buf[MAXSIZE];
    int i;
    int ret;

    if(argc != 2)
    {
        printf("usage: od filename\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("fopen failed");
        exit(1);
    }

    while((ret = read(fd, buf, MAXSIZE)) > 0)
    {
        for(i=0;i<ret;i++)
        {
            printf("%02x ", buf[i]);
        }
        printf("\n");

        for(i=0;i<ret;i++)
        {
            printf("%c  ", buf[i]);
        }
        printf("\n");
    }
    close(fd);
    return 0;
}
