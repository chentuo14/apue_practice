#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 512
#define MAXLINE 10

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[BUFSIZE];
    char *temp[BUFSIZE];
    int lineCount = 0;
    int i = 0, j = 0;

    if(argc != 2)
    {
        printf("usage: tail filename\n");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("fopen failed");
        exit(1);
    }
    while(fgets(buf, BUFSIZE, fp) != NULL)
    {
        lineCount++;
    }

    fclose(fp);
    fp = fopen(argv[1], "r");

    while(fgets(buf, BUFSIZE, fp) != NULL)
    {
        i++;
        if(i>(lineCount-MAXLINE))
            fprintf(stdout, "%s", buf);
    }

    printf("\n");
    fclose(fp);
    return 0; 
}