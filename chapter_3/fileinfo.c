/* fileinfo.c - use stat() to obtain and print file properties
 *            - some members are just numbers...
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
    struct stat info;

    if(argc>1)
    if(stat(argv[1], &info) != -1)
    {
        show_stat_info(argv[1], &info);
        return 0;
    }
    else
        perror(argv[1]);
    return 1;
}

show_stat_info(char *fname, struct stat *buf)
{
    printf("    mode: %o\n", buf->st_mode);
    printf("    nlink: %d\n", buf->st_nlink);
    printf("    user: %d\n", buf->st_uid);
    printf("    group: %d\n", buf->st_gid);
    printf("    size: %d\n", buf->st_size);
    printf("    modtime: %d\n", buf->st_mtime);
    printf("    name: %s\n", fname);
}

void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");

    if(S_ISDIR(mode))   str[0] = 'd';   /* directory? */
    if(S_ISCHR(mode))   str[0] = 'c';   /* character device? */
    if(S_ISBLK(mode))   str[0] = 'b';   /* block device? */

    if(mode & S_IRUSR)  str[1] = 'r';
    if(mode & S_IWUSR)  str[2] = 'w';
    if(mode & S_IXUSR)  str[3] = 'x';

    if(mode & S_IRGRP)  str[4] = 'r';
    if(mode & S_IWGRP)  str[5] = 'w';
    if(mode & S_IXGRP)  str[6] = 'x';

    if(mode & S_IROTH)  str[7] = 'r';
    if(mode & S_IWOTH)  str[8] = 'w';
    if(mode & S_IXOTH)  str[9] = 'x';
}

char *uid_to_name(uid_t uid)
{
    struct passwd *passwdp;
    passwdp = getpwuid(uid);
    if(passwdp != NULL)
        return (passwdp->pw_name);
    else 
        return NULL;
}
