#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAXLINE 256

struct MyStruct {
    int id;
    int read_fd;
};

void* readFunc(void* argument)
{
    struct MyStruct* info = (struct MyStruct*)argument;
    int n;
    char inmsg[MAXLINE];

    while (1)
    {
        n = read(info->read_fd, inmsg, MAXLINE);
        if (n <= 0)
        {
            perror("read");
            exit(1);
        }
        printf("\n%s\n", inmsg);
    }
}

int main()
{
    pthread_t thread;
    int fd1, fd2;
    char inmsg[MAXLINE];

    fd1 = open("./cfifo1", O_WRONLY);
    fd2 = open("./cfifo4", O_RDONLY);

    if (fd1 == -1 || fd2 == -1)
    {
        perror("open");
        exit(1);
    }

    struct MyStruct info = {1, fd2};
    pthread_create(&thread, NULL, readFunc, (void*)&info);

    while (1)
    {
        fgets(inmsg, MAXLINE, stdin);
        write(fd1, inmsg, strlen(inmsg) + 1);
    }

    pthread_join(thread, NULL);

    return 0;
}
