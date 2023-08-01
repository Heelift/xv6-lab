#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int fd[2];

    if (pipe(fd) == -1) {
        fprintf(2, "pipe error\n");
    }

    //子进程
    if (fork() == 0){
        char buffer[1];
        read(fd[0], buffer, 1);
        close(fd[0]); //关闭读端
        fprintf(0, "%d: received ping\n", getpid());
        write(fd[1], buffer, 1);
        close(fd[1]); //关闭写端
    }
    //父进程
    else {
        char buffer[1];
        buffer[0] = 'a';
        write(fd[1], buffer, 1);
        close(fd[1]); //关闭写端
        read(fd[0], buffer, 1);
        close(fd[0]); //关闭读端
        fprintf(0, "%d: received pong\n", getpid());
    }

    exit(0);
}
