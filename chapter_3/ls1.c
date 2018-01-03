#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

void do_ls(char *);

int main(int argc, int argv[])
{
    if(argc == 1)
        do_ls(".");
    else
    {
        while(--argc)
        {
            printf("%s:\n", *++argv);
            do_ls(*argv);
        }
    }
}

void do_ls(char *dirname)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    dir_ptr = opendir(dirname);
    if(dir_ptr == NULL)
        fprintf(stderr, "ls1:cannot open %s\n", dirname);
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
            printf("%s ", direntp->d_name);
        printf("\n");
        closedir(dir_ptr);
    }
}