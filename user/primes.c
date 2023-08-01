#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p[])
{
    close(p[1]);

    int i;
    if (read(p[0], &i, sizeof(i)) == 0)
    {
        close(p[0]);
        exit(0);
    }

    printf("prime %d\n", i);
    int num = 0;
    int fd[2];
    pipe(fd);
    
    if (fork() == 0)
    {
        primes(fd);
    }
    else
    {
        close(fd[0]);
        while (read(p[0], &num, sizeof(num)) > 0)
        {
            if (num % i != 0)
            {
                write(fd[1], &num, sizeof(num));
            }
        }
        close(fd[1]);
        wait(0);
    }

    exit(0);
}


int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    
    if (fork() == 0)
    {
        primes(p);
    }
    else
    {
        close(p[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }

    exit(0);
}
