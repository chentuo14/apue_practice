#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    struct stat infobuf;
    int ret;

    ret = stat("/etc/passwd", &infobuf);
    if(ret == -1)
        perror("/etc/passwd");
    else
    {
        printf("the size of /etc/passwd is %d\n", infobuf.st_size);
    }
}