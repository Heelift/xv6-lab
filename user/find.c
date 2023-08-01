#include "kernel/types.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/stat.h"

char *fmtname(char *path)
{
    char *p;
    p = path + strlen(path);
    while(p >= path && *p != '/'){
        p--;
    }
    p++;
    return p;
}

void print(char *path, char *findname)
{
    if (strcmp(fmtname(path), findname) == 0)
        printf("%s\n", path);
}

void find(char *dir_name, char *file_name)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;

    if ((fd = open(dir_name, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", dir_name);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", dir_name);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        print(dir_name, file_name);
        break;
    case T_DIR:
        if (strlen(dir_name) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, dir_name);
        p = buf + strlen(buf); //p鎸囧悜path鐨勬湯灏?
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0))
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, file_name);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: find path file.\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}